#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

int max_fd = 0, count = 0, clients[65000];
char read_buf[1001], write_buf[1001], *msgs[65000]; // Don't forget that msgs is a 2D matrix, NOT a string!!!
fd_set readfds, writefds, all;

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
	// socket create and verification 
	max_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (max_fd < 0) ft_err(NULL);
	FD_SET(max_fd, &all);
	return (max_fd);
}

void notify(int author, char *msg) {
	for (int fd = 0; fd <= max_fd; fd++) {
		if (FD_ISSET(fd, &writefds) && fd != author) send(fd, msg, strlen(msg), 0);
	}
}

void make_client(int fd) {
	max_fd = fd > max_fd ? fd : max_fd;
	FD_SET(fd, &all);
	clients[fd] = count++;
	msgs[fd] = NULL;
	sprintf(write_buf, "server: client %d just arrived\n", clients[fd]);
	notify(fd, write_buf);
}

void remove_client(int fd) { //Remember that remove_client needs to be written in reverse compared to make_client
	sprintf(write_buf, "server: client %d just left\n", clients[fd]);
	notify(fd, write_buf);
	free(msgs[fd]);
	FD_CLR(fd, &all);
	close(fd);
}

void send_msg(int fd) {
	char *msg;
	while(extract_message(&(msgs[fd]), &msg) != 0) {
		sprintf(write_buf, "client %d: ", clients[fd]);
		notify(fd, write_buf);
		notify(fd, msg);
		free(msg);
	}
}

int main(int ac, char **av) {
	int sockfd;
	struct sockaddr_in servaddr; 
	
	printf("[1/7] Check for args...\n");
	if (ac != 2) ft_err("Wrong number of arguments");
	
	printf("[2/7] Initializing all and servaddr...\n");
	FD_ZERO(&all); // Remember, this is the first thing that needs to be done; FD_ZERO is used to initialize all;
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); 
	
	printf("[3/7] Creating socket...\n");
	sockfd = make_socket();
  
	printf("[4/7] Binding newly created socket to given IP and verification...\n");
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) ft_err(NULL);
	if (listen(sockfd, 100) != 0) ft_err(NULL);
	
	while (1) {
		readfds = writefds = all; // If the code doesn't seem to react to the nc command, you proably forgot this line
		printf("[5/7] Checking for available clients...\n");
		if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0) ft_err(NULL);
		for (int fd = 0; fd <= max_fd; fd++) {
			// if (FD_ISSET(fd, &all)) continue; //This is wrong!!
			if (FD_ISSET(fd, &readfds) == 0) continue; // This is the correct line!! Here you need to remember: 1) This line checks if fd is found in readfds (not all); it should ignore the fd that is not present in readfds (by checking if the macro returns 0; if it does, then fd is not in readfds and the code should continue by ignoring the fd
			if (fd == sockfd) {
				socklen_t len = sizeof(servaddr);
				int client_id = accept(sockfd, (struct sockaddr *)&servaddr, &len);
				if (client_id >= 0) {
					printf("[6/7] Creating client %d...\n", client_id);
					make_client(client_id);
					break;
				}
			} else {
				int read_bytes = recv(fd, &read_buf, 1000, 0); //Please be careful, recv needs to receive from fd, NOT! from sockfd
				if (read_bytes <= 0) {
					printf("[7/7] Nothing to read. Removing client %d...\n", fd);
					remove_client(fd);
					break;
				}
				read_buf[read_bytes] = '\0';
				msgs[fd] = str_join(msgs[fd], read_buf);
				send_msg(fd);
			}
		}
	}
	return (0);
}