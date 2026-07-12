#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

enum { MAX_FD = 65536, BUF_SIZE = 1000 };

// Listening socket descriptor.
int sockfd = 0;
// Highest descriptor currently tracked by select().
int max_fd = 0;
// Next client id to assign on accept().
int next_id = 0;
// Client id indexed by file descriptor.
int ids[MAX_FD];
// Per-client receive buffer holding incomplete lines.
char *inbuf[MAX_FD];
// Per-client send queue holding pending outbound bytes.
char *outbuf[MAX_FD];
// Master fd set + temporary read/write sets copied before each select().
fd_set all, readfds, writefds;

void remove_client(int fd);

// Print one mandatory subject error on stderr then exit(1).
void ft_err(char *str)
{
	// If caller provided a message, print it.
	if (str)
		write(2, str, strlen(str));
	// Otherwise print the generic fatal message.
	else
		write(2, "Fatal error", strlen("Fatal error"));
	// Subject requires a trailing newline.
	write(2, "\n", 1);
	// Hard-stop process after fatal condition.
	exit(1);
}

// Allocate and return concatenation of s1 + s2.
// Returns NULL on allocation failure.
char *join(char *s1, char *s2)
{
	char	*res;
	size_t	len1;
	size_t	len2;

	// Default to 0 when one side is missing.
	len1 = 0;
	len2 = 0;
	// Measure first string if present.
	if (s1)
		len1 = strlen(s1);
	// Measure second string if present.
	if (s2)
		len2 = strlen(s2);
	// Allocate output (+1 for '\0').
	res = calloc(1, len1 + len2 + 1);
	if (!res)
		return (0);
	// Copy first part if present.
	if (s1)
		strcpy(res, s1);
	// Append second part if present.
	if (s2)
		strcat(res, s2);
	return (res);
}

// Try to split one full line from *buf.
// On success: returns 1, stores full line (with '\n') in *msg,
// and keeps remaining tail in *buf.
// If no full line exists yet: returns 0.
// On allocation failure: returns -1.
int extract_message(char **buf, char **msg)
{
	char	*newline;
	char	*rest;
	size_t	prefix_len;
	size_t	rest_len;

	// Default output to NULL.
	*msg = 0;
	// Nothing to parse.
	if (!buf || !*buf)
		return (0);
	// Look for first newline delimiter.
	newline = strstr(*buf, "\n");
	if (!newline)
		return (0);
	// Bytes before '\n'.
	prefix_len = (size_t)(newline - *buf);
	// Bytes after '\n'.
	rest_len = strlen(newline + 1);
	// Temporarily terminate at newline to copy prefix safely.
	*newline = '\0';
	// +2 => one for newline restored in msg, one for '\0'.
	*msg = calloc(1, prefix_len + 2);
	// Allocate storage for remaining tail.
	rest = calloc(1, rest_len + 1);
	if (!*msg || !rest)
	{
		free(*msg);
		free(rest);
		*msg = 0;
		// Restore original buffer before returning failure.
		*newline = '\n';
		return (-1);
	}
	// Build full extracted line "<prefix>\n".
	strcpy(*msg, *buf);
	strcat(*msg, "\n");
	// Copy remaining bytes after newline.
	strcpy(rest, newline + 1);
	// Replace old input buffer by remaining tail.
	free(*buf);
	*buf = rest;
	return (1);
}

// Append msg to outbuf[fd] (send queue for that client).
void queue_message(int fd, char *msg)
{
	char	*tmp;

	// Build concatenated queue.
	tmp = join(outbuf[fd], msg);
	if (!tmp)
		ft_err(0);
	// Swap in newly combined queue.
	free(outbuf[fd]);
	outbuf[fd] = tmp;
}

// Send one message to all connected clients except author.
void broadcast_others(int author, char *msg)
{
	int	fd;

	fd = 0;
	while (fd <= max_fd)
	{
		// Skip listening socket and message author.
		// FD_ISSET(fd, &all) means this descriptor is active.
		if (fd != sockfd && fd != author && FD_ISSET(fd, &all))
			queue_message(fd, msg);
		fd++;
	}
}

// Attempt to send pending queued bytes for one client.
// Handles partial send() by keeping unsent tail in outbuf[fd].
void flush_client(int fd)
{
	char	*rest;
	int		sent;
	size_t	len;

	// Keep trying while this client still has queued data.
	while (outbuf[fd])
	{
		// Current queued payload size.
		len = strlen(outbuf[fd]);
		// Try to send what is currently queued.
		sent = send(fd, outbuf[fd], len, 0);
		// send <= 0 => peer likely gone / fatal socket state.
		if (sent <= 0)
		{
			remove_client(fd);
			return ;
		}
		// Full queue sent: free it and stop for this cycle.
		if ((size_t)sent == len)
		{
			free(outbuf[fd]);
			outbuf[fd] = 0;
			return ;
		}
		// Partial send: keep only the remaining bytes unsent.
		rest = calloc(1, len - (size_t)sent + 1);
		if (!rest)
			ft_err(0);
		strcpy(rest, outbuf[fd] + sent);
		free(outbuf[fd]);
		outbuf[fd] = rest;
	}
}

// Disconnect one client and broadcast departure to others.
void remove_client(int fd)
{
	char msg[64];

	// Build and broadcast leave message before descriptor cleanup.
	sprintf(msg, "server: client %d just left\n", ids[fd]);
	broadcast_others(fd, msg);
	// Release per-client buffers.
	free(inbuf[fd]);
	free(outbuf[fd]);
	inbuf[fd] = 0;
	outbuf[fd] = 0;
	// Remove descriptor from master select set and close fd.
	FD_CLR(fd, &all);
	close(fd);
}

// Register accepted client and broadcast arrival to others.
void add_client(int fd)
{
	char msg[64];

	// Keep max_fd updated for select(max_fd + 1, ...).
	if (fd > max_fd)
		max_fd = fd;
	// Add descriptor to tracked set.
	FD_SET(fd, &all);
	// Assign monotonic id then increment id counter.
	ids[fd] = next_id;
	next_id++;
	// Start with empty in/out buffers.
	inbuf[fd] = 0;
	outbuf[fd] = 0;
	// Inform other connected clients.
	sprintf(msg, "server: client %d just arrived\n", ids[fd]);
	broadcast_others(fd, msg);
}

// Read incoming bytes from one client, split by '\n', and broadcast each line.
void handle_read(int fd)
{
	char	buffer[BUF_SIZE + 1];
	char	*msg;
	char	prefix[64];
	char	*line;
	int		read_bytes;
	int		res;

	// Receive up to BUF_SIZE bytes from this client.
	read_bytes = recv(fd, buffer, BUF_SIZE, 0);
	// recv <= 0 means disconnect or socket error => drop client.
	if (read_bytes <= 0)
	{
		remove_client(fd);
		return ;
	}
	// Null-terminate so we can use string helpers.
	buffer[read_bytes] = '\0';
	// Append newly received data to this client's pending input buffer.
	inbuf[fd] = join(inbuf[fd], buffer);
	if (!inbuf[fd])
		ft_err(0);
	// Extract and broadcast every complete newline-terminated message.
	while (1)
	{
		res = extract_message(&inbuf[fd], &msg);
		if (res < 0)
			ft_err(0);
		// No complete line left yet.
		if (res == 0)
			break;
		// Prefix with sender client id.
		sprintf(prefix, "client %d: ", ids[fd]);
		// Build final payload "<prefix><line>".
		line = join(prefix, msg);
		if (!line)
			ft_err(0);
		// Send to every other connected client.
		broadcast_others(fd, line);
		// Free temporary allocations for this extracted line.
		free(line);
		free(msg);
	}
}

// Create listening socket and register it in master fd set.
void make_socket(void)
{
	// TCP IPv4 socket.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		ft_err(0);
	// Track listening descriptor in select() set.
	FD_SET(sockfd, &all);
	if (sockfd > max_fd)
		max_fd = sockfd;
}

int main(int ac, char **av)
{
	struct sockaddr_in addr;
	int fd;
	int client_fd;

	// Subject requires exactly one argument: port.
	if (ac != 2) ft_err("Wrong number of arguments");
	// Clear master descriptor set before first use.
	FD_ZERO(&all);
	// Zero-initialize socket address struct.
	bzero(&addr, sizeof(addr));
	// Create listening socket and add to fd set.
	make_socket();
	// IPv4 loopback socket configuration.
	addr.sin_family = AF_INET;
	// 127.0.0.1 in network byte order.
	addr.sin_addr.s_addr = htonl(2130706433);
	// Port from argv[1] in network byte order.
	addr.sin_port = htons(atoi(av[1]));
	// Bind listening socket to requested local endpoint.
	if (bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
		ft_err(0);
	// Start accepting incoming connections.
	if (listen(sockfd, 100) < 0)
		ft_err(0);
	// Main event loop:
	// - wait for readable sockets with select()
	// - accept new clients
	// - read and buffer client input
	// - flush any queued outgoing data without blocking the server
	while (1)
	{
		// select() mutates sets, so copy from master before each call.
		readfds = all;
		writefds = all;
		// Wait until at least one fd is ready for read/write.
		if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0)
			ft_err(0);
		fd = 0;
		while (fd <= max_fd)
		{
			// Process readable descriptors first.
			if (FD_ISSET(fd, &readfds))
			{
				// Readable listening socket => one pending incoming client.
				if (fd == sockfd)
				{
					socklen_t len = sizeof(addr);

					client_fd = accept(sockfd, (struct sockaddr *)&addr, &len);
					// Register client only if accept succeeded.
					if (client_fd >= 0)
						add_client(client_fd);
				}
				// Otherwise this is an existing client with incoming data.
				else
					handle_read(fd);
			}
			fd++;
		}
		fd = 0;
		while (fd <= max_fd)
		{
			// Flush queued output only for client fds marked writable.
			if (fd != sockfd && FD_ISSET(fd, &writefds) && outbuf[fd])
				flush_client(fd);
			fd++;
		}
	}
	return (0);
}
