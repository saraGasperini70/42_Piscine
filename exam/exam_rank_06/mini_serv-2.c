#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Allowed functions: write, close, select, poll, socket, accept, listen, send, recv, bind, strstr, malloc, realloc, free, calloc, bzero, atoi, sprintf, strlen, exit, strcpy, strcat, memset, htons, htonl

int maxfd = 0, ids[65000], count = 0;
fd_set all, writefds, readfds;
char *msgs[65000] writefds[1001], readfds[1001];

int extract_message(char **buf, char **msg)
{
        char        *newbuf;
        int        i;

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
        char        *newbuf;
        int                len;

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

int socket_create() {
	maxfd = socket(AF_INET, SOCK_STREAM, 0);
	if (maxfd < 0) ft_err();
	FD_SET(maxfd, &all);
	return maxfd;
}

int make_client(int fd) {
	// 1. set maxfd
	// 2. fd set
	// 3. add count++ to fds[fd]
	// 4. set msgs[fd] to NULL
	// 5. add the message to buf_write (the arrival of the new client)
	// 6. broadcast buf_write
}

void remove_client(int fd) {
	// 1. add the message to buf_write (client just left)
	// 2. broadcast buf_write
	// 3. free msgs[fd]
	// 4. clear fd, &all
	// 5. close fd
}

void send_msg(int fd) {
}


int main(int ac, char **av) {
        int sockfd;
        struct sockaddr_in servaddr; 
	
	if (ac != 2) ft_err("Wrong number of arguments");

        // socket create and verification
	FD_ZERO(&all);
	bzero(&servaddr, sizeof(servaddr));
        sockfd = socket_create(); 

        // assign IP, PORT 
        servaddr.sin_family = AF_INET; 
        servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
        servaddr.sin_port = htons(atoi(av[1])); 

        // Binding newly created socket to given IP and verification 
        if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) ft_err();
        if (listen(sockfd, 100) != 0) ft_err()
        
	while(1) {
		readfds = writefds = all;
		if (select(maxfd + 1, &readfds, &writefds, NULL, NULL) < 0) ft_err();
		for (int fd = 0; fd <= maxfd; fd++) {
			if (FD_ISSET(fd, &readfd) == 0) continue;
			if (fd == sockfd) {
				socklen_t addr_len = sizeof(servaddr);
				int client_id = accept(sockfd, (struct sockaddr *)&servaddr, &addr_len);
				if (client_id >= 0) {
					make_client(client_id);
					break;
				}
			}
			else {
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
