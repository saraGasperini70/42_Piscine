#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

int clients[65000];
char *msgs[65000];
fd_set all, writefds, readfds;
int max_fd = 0, count = 0;
char buf_write[1001], buf_read[1001];

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void ft_err(char *msg) {
	if (msg) write(2, msg, strlen(msg));
	else write(2, "Fatal error", strlen("Fatal error"));
	write(2, "\n", 1);
	exit(1);
}

int make_socket() {
	max_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (max_fd < 0) ft_err(NULL);
	FD_SET(max_fd, &all);
	return (max_fd);
}

void notify(int author, char *msg) {
	for (int fd = 0; fd <= max_fd; fd++) {
		if (FD_ISSET(fd, &writefds) && fd != author) send(fd, &msg, strlen(msg), 0);
	}
}

void make_client(int client_id) {
	max_fd = client_id > max_fd ? client_id : max_fd;
	FD_SET(client_id, &all);
	clients[client_id] = count++;
	msgs[client_id] = NULL;
	sprintf(buf_write, "server: client %d just arrived\n", clients[client_id]);
	notify(client_id, buf_write);
}

void remove_client(int client_id) {
	sprintf(buf_write, "server: client %d just left\n", clients[client_id]);
	notify(client_id, buf_write);
	free(msgs[client_id]);
	FD_CLR(client_id, &all);
	close(client_id);
}

void send_msg(int client_id) {
	char *msg;
	while (extract_message(&(msgs[client_id]), &msg)) {
		sprintf(buf_write, "client %d: ", clients[client_id]);
		notify(client_id, buf_write);
		notify(client_id, msg);
		free(msg);
	}
}

void debug(char *msg) {
	write(1, msg, strlen(msg));
	write(1, "\n", 1);
}

int main(int ac, char **av) {
	struct sockaddr_in servaddr;

	// Check arguments
	if (ac != 2) ft_err("Wrong number of arguments");

	FD_ZERO(&all);
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));

	// socket create and verification
	int sockfd = make_socket();

	// Binding newly created socket to given IP and verification
	debug("Binding socket...");
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) ft_err(NULL);
	debug("Success. Socket listening...");
	if (listen(sockfd, 100) != 0) ft_err(NULL);
	debug("Done.");

	while (1) {
		readfds = writefds = all;
		if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0) ft_err(NULL);
		for (int fd = 0; fd <= max_fd; fd++) {
			if (FD_ISSET(fd, &readfds) == 0) continue;
			if (fd == sockfd) {
				socklen_t len = sizeof(servaddr);
				int client_id = accept(sockfd, (struct sockaddr *)&servaddr, &len);
				if (client_id >= 0) {
					make_client(client_id);
					break;
				}
			} else {
					int read_bytes = recv(fd, buf_read, 1000, 0);
					if (read_bytes <= 0) {
						remove_client(fd);
						break;
					}
					buf_read[read_bytes] = '\0';
					msgs[fd] = str_join(msgs[fd], buf_read);
					send_msg(fd);
				}
			}
		}
	return (0);
}
