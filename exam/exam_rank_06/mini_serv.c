#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

enum { MAX_FD = 65536, BUF_SIZE = 1000 };

// Global server state.
// sockfd is the listening socket.
// max_fd tracks the highest descriptor currently in use.
// next_id assigns incremental ids to new clients.
// ids[fd] stores the client id for a socket.
// inbuf[fd] keeps unfinished incoming data for that client.
// outbuf[fd] keeps data that still needs to be sent to that client.
int sockfd = 0;
int max_fd = 0;
int next_id = 0;
int ids[MAX_FD];
char *inbuf[MAX_FD];
char *outbuf[MAX_FD];
fd_set all, readfds, writefds;

void remove_client(int fd);

// Print the required error message and exit immediately.
void ft_err(char *str)
{
	if (str)
		write(2, str, strlen(str));
	else
		write(2, "Fatal error", strlen("Fatal error"));
	write(2, "\n", 1);
	exit(1);
}

// Join two strings into a newly allocated buffer.
// This is used both for received input and for queued outgoing messages.
char *join(char *s1, char *s2)
{
	char	*res;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = strlen(s1);
	if (s2)
		len2 = strlen(s2);
	res = calloc(1, len1 + len2 + 1);
	if (!res)
		return (0);
	if (s1)
		strcpy(res, s1);
	if (s2)
		strcat(res, s2);
	return (res);
}

// Extract one full line ending in '\n' from *buf.
// The extracted line is returned in *msg, and *buf is replaced with the rest.
int extract_message(char **buf, char **msg)
{
	char	*newline;
	char	*rest;
	size_t	prefix_len;
	size_t	rest_len;

	*msg = 0;
	if (!buf || !*buf)
		return (0);
	newline = strstr(*buf, "\n");
	if (!newline)
		return (0);
	prefix_len = (size_t)(newline - *buf);
	rest_len = strlen(newline + 1);
	*newline = '\0';
	*msg = calloc(1, prefix_len + 2);
	rest = calloc(1, rest_len + 1);
	if (!*msg || !rest)
	{
		free(*msg);
		free(rest);
		*msg = 0;
		*newline = '\n';
		return (-1);
	}
	strcpy(*msg, *buf);
	strcat(*msg, "\n");
	strcpy(rest, newline + 1);
	free(*buf);
	*buf = rest;
	return (1);
}

// Append a message to a client's outgoing queue.
void queue_message(int fd, char *msg)
{
	char	*tmp;

	tmp = join(outbuf[fd], msg);
	if (!tmp)
		ft_err(0);
	free(outbuf[fd]);
	outbuf[fd] = tmp;
}

// Broadcast a message to everyone except the sender and the listening socket.
void broadcast_others(int author, char *msg)
{
	int	fd;

	fd = 0;
	while (fd <= max_fd)
	{
		if (fd != sockfd && fd != author && FD_ISSET(fd, &all))
			queue_message(fd, msg);
		fd++;
	}
}

// Try to flush as much queued output as possible for one client.
// If send() writes only part of the buffer, keep the remaining tail queued.
// If the socket is no longer valid, remove the client right away.
void flush_client(int fd)
{
	char	*rest;
	int		sent;
	size_t	len;

	while (outbuf[fd])
	{
		len = strlen(outbuf[fd]);
		sent = send(fd, outbuf[fd], len, 0);
		if (sent <= 0)
		{
			remove_client(fd);
			return ;
		}
		if ((size_t)sent == len)
		{
			free(outbuf[fd]);
			outbuf[fd] = 0;
			return ;
		}
		rest = calloc(1, len - (size_t)sent + 1);
		if (!rest)
			ft_err(0);
		strcpy(rest, outbuf[fd] + sent);
		free(outbuf[fd]);
		outbuf[fd] = rest;
	}
}

// Remove a client, broadcast its departure, and release all resources for it.
void remove_client(int fd)
{
	char msg[64];

	sprintf(msg, "server: client %d just left\n", ids[fd]);
	broadcast_others(fd, msg);
	free(inbuf[fd]);
	free(outbuf[fd]);
	inbuf[fd] = 0;
	outbuf[fd] = 0;
	FD_CLR(fd, &all);
	close(fd);
}

// Register a new client and announce its arrival to everybody else.
void add_client(int fd)
{
	char msg[64];

	if (fd > max_fd)
		max_fd = fd;
	FD_SET(fd, &all);
	ids[fd] = next_id;
	next_id++;
	inbuf[fd] = 0;
	outbuf[fd] = 0;
	sprintf(msg, "server: client %d just arrived\n", ids[fd]);
	broadcast_others(fd, msg);
}

// Read data from one client, accumulate it, and rebroadcast every full line.
void handle_read(int fd)
{
	char	buffer[BUF_SIZE + 1];
	char	*msg;
	char	prefix[64];
	char	*line;
	int		read_bytes;
	int		res;

	read_bytes = recv(fd, buffer, BUF_SIZE, 0);
	if (read_bytes <= 0)
	{
		remove_client(fd);
		return ;
	}
	buffer[read_bytes] = '\0';
	inbuf[fd] = join(inbuf[fd], buffer);
	if (!inbuf[fd])
		ft_err(0);
	while (1)
	{
		res = extract_message(&inbuf[fd], &msg);
		if (res < 0)
			ft_err(0);
		if (res == 0)
			break;
		sprintf(prefix, "client %d: ", ids[fd]);
		line = join(prefix, msg);
		if (!line)
			ft_err(0);
		broadcast_others(fd, line);
		free(line);
		free(msg);
	}
}

// Create the listening socket and add it to the active descriptor set.
void make_socket(void)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		ft_err(0);
	FD_SET(sockfd, &all);
	if (sockfd > max_fd)
		max_fd = sockfd;
}

int main(int ac, char **av)
{
	struct sockaddr_in addr;
	int fd;
	int client_fd;

	if (ac != 2) ft_err("Wrong number of arguments");
	FD_ZERO(&all);
	bzero(&addr, sizeof(addr));
	make_socket();
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(2130706433);
	addr.sin_port = htons(atoi(av[1]));
	if (bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
		ft_err(0);
	if (listen(sockfd, 100) < 0)
		ft_err(0);
	// Main event loop:
	// - wait for readable sockets with select()
	// - accept new clients
	// - read and buffer client input
	// - flush any queued outgoing data without blocking the server
	while (1)
	{
		readfds = all;
		writefds = all;
		if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0)
			ft_err(0);
		fd = 0;
		while (fd <= max_fd)
		{
			if (FD_ISSET(fd, &readfds))
			{
				if (fd == sockfd)
				{
					socklen_t len = sizeof(addr);

					client_fd = accept(sockfd, (struct sockaddr *)&addr, &len);
					if (client_fd >= 0)
						add_client(client_fd);
				}
				else
					handle_read(fd);
			}
			fd++;
		}
		fd = 0;
		while (fd <= max_fd)
		{
			if (fd != sockfd && FD_ISSET(fd, &writefds) && outbuf[fd])
				flush_client(fd);
			fd++;
		}
	}
	return (0);
}
