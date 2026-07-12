#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
** Subject constraints:
** - one file: mini_serv.c
** - no #define allowed -> use enum constants
*/
enum { MAX_FD = 65536, BUF_SIZE = 1000 };

/*
** Global server state
** - sockfd: listening socket
** - max_fd: highest fd currently tracked (for select)
** - next_id: incremental client id (0,1,2,...)
** - ids[fd]: client id associated with fd
** - inbuf[fd]: accumulated incoming data not fully processed yet
** - outbuf[fd]: queued outgoing data not fully sent yet
** - all: master fd set (listening + connected clients)
*/
int     sockfd;
int     max_fd;
int     next_id;
int     ids[MAX_FD];
char    *inbuf[MAX_FD];
char    *outbuf[MAX_FD];
fd_set  all, readfds, writefds;

/* Forward declaration (used by flush_client on send failure). */
void    remove_client(int fd);

/*
** Print exact required errors and exit(1).
** - If custom string provided, print it
** - Else print "Fatal error"
*/
void    ft_err(char *str)
{
    if (str)
        write(2, str, strlen(str));
    else
        write(2, "Fatal error", 11);
    write(2, "\n", 1);
    exit(1);
}

/*
** Allocate and return concatenation of s1 + s2.
** Returns NULL on allocation failure.
*/
char    *join(char *s1, char *s2)
{
    size_t  l1 = s1 ? strlen(s1) : 0;
    size_t  l2 = s2 ? strlen(s2) : 0;
    char    *res = calloc(1, l1 + l2 + 1);

    if (!res)
        return NULL;
    if (s1)
        strcpy(res, s1);
    if (s2)
        strcat(res, s2);
    return res;
}

/*
** Extract one full line ending with '\n' from *buf.
**
** Example:
**   *buf = "abc\ndef\nxyz"
**   -> msg = "abc\n", *buf becomes "def\nxyz", returns 1
**
** Returns:
**   1  -> one message extracted
**   0  -> no full line yet
**  -1  -> allocation failure
*/
int     extract_message(char **buf, char **msg)
{
    char    *nl;
    char    *rest;
    size_t  prefix_len;
    size_t  rest_len;

    *msg = NULL;
    if (!buf || !*buf)
        return 0;
    nl = strstr(*buf, "\n");
    if (!nl)
        return 0;

    prefix_len = (size_t)(nl - *buf);
    rest_len = strlen(nl + 1);

    *nl = '\0';
    *msg = calloc(1, prefix_len + 2);   /* + '\n' + '\0' */
    rest = calloc(1, rest_len + 1);
    if (!*msg || !rest)
    {
        free(*msg);
        free(rest);
        *msg = NULL;
        *nl = '\n';
        return -1;
    }

    strcpy(*msg, *buf);
    strcat(*msg, "\n");
    strcpy(rest, nl + 1);

    free(*buf);
    *buf = rest;
    return 1;
}

/*
** Append 'msg' to outbuf[fd].
** Uses temp pointer to avoid memory leaks on replacement.
*/
void    queue_message(int fd, char *msg)
{
    char    *tmp = join(outbuf[fd], msg);

    if (!tmp)
        ft_err(NULL);
    free(outbuf[fd]);
    outbuf[fd] = tmp;
}

/*
** Queue a message to all connected clients except:
** - the listening socket
** - the author (sender)
*/
void    broadcast_others(int author, char *msg)
{
    int fd;

    for (fd = 0; fd <= max_fd; fd++)
    {
        if (fd != sockfd && fd != author && FD_ISSET(fd, &all))
            queue_message(fd, msg);
    }
}

/*
** Try to send queued output for one client.
**
** Behavior:
** - If all queued data sent: clear outbuf[fd]
** - If partial send: keep unsent tail in outbuf[fd]
** - If send <= 0: treat as disconnected and remove client
**
** Important for subject:
** - Do NOT disconnect lazy clients just because they are slow.
** - Keep pending data and retry later when writable.
*/
void    flush_client(int fd)
{
    int     sent;
    size_t  len;
    char    *rest;

    while (outbuf[fd])
    {
        len = strlen(outbuf[fd]);
        sent = send(fd, outbuf[fd], len, 0);
        if (sent <= 0)
        {
            remove_client(fd);
            return;
        }
        if ((size_t)sent == len)
        {
            free(outbuf[fd]);
            outbuf[fd] = NULL;
            return;
        }

        rest = calloc(1, len - (size_t)sent + 1);
        if (!rest)
            ft_err(NULL);
        strcpy(rest, outbuf[fd] + sent);
        free(outbuf[fd]);
        outbuf[fd] = rest;
    }
}

/*
** Remove one client:
** - announce departure to others
** - free client buffers
** - remove fd from master set
** - close fd
*/
void    remove_client(int fd)
{
    char msg[64];

    sprintf(msg, "server: client %d just left\n", ids[fd]);
    broadcast_others(fd, msg);

    free(inbuf[fd]);
    free(outbuf[fd]);
    inbuf[fd] = NULL;
    outbuf[fd] = NULL;

    FD_CLR(fd, &all);
    close(fd);
}

/*
** Register new client:
** - track fd in master set
** - assign id
** - init buffers
** - announce arrival to others
*/
void    add_client(int fd)
{
    char msg[64];

    if (fd > max_fd)
        max_fd = fd;
    FD_SET(fd, &all);

    ids[fd] = next_id++;
    inbuf[fd] = NULL;
    outbuf[fd] = NULL;

    sprintf(msg, "server: client %d just arrived\n", ids[fd]);
    broadcast_others(fd, msg);
}

/*
** Read incoming bytes from one client and process complete lines.
**
** Steps:
** 1) recv chunk
** 2) append chunk to inbuf[fd]
** 3) extract each full line ending in '\n'
** 4) broadcast each line as "client <id>: <line>"
**
** "As fast as possible" requirement is satisfied by immediate line forwarding.
*/
void    handle_read(int fd)
{
    char    buf[BUF_SIZE + 1];
    int     n;
    char    *tmp;
    char    *msg;
    char    *line;
    char    prefix[64];
    int     status;

    n = recv(fd, buf, BUF_SIZE, 0);
    if (n <= 0)
    {
        remove_client(fd);
        return;
    }
    buf[n] = '\0';

    /* Leak-safe append (fix vs direct inbuf[fd] = join(...)). */
    tmp = join(inbuf[fd], buf);
    if (!tmp)
        ft_err(NULL);
    free(inbuf[fd]);
    inbuf[fd] = tmp;

    while (1)
    {
        status = extract_message(&inbuf[fd], &msg);
        if (status < 0)
            ft_err(NULL);
        if (status == 0)
            break;

        sprintf(prefix, "client %d: ", ids[fd]);
        line = join(prefix, msg);
        if (!line)
            ft_err(NULL);

        broadcast_others(fd, line);

        free(line);
        free(msg);
    }
}

int     main(int ac, char **av)
{
    struct sockaddr_in  addr;
    int                 fd;
    int                 cfd;
    socklen_t           alen;

    if (ac != 2)
        ft_err("Wrong number of arguments");

    FD_ZERO(&all);
    memset(&addr, 0, sizeof(addr));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        ft_err(NULL);

    FD_SET(sockfd, &all);
    max_fd = sockfd;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433); /* 127.0.0.1 */
    addr.sin_port = htons(atoi(av[1]));

    if (bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
        ft_err(NULL);
    if (listen(sockfd, 100) < 0)
        ft_err(NULL);

    /*
    ** Main event loop:
    ** - select() tells which fds are readable/writable now
    ** - readable server fd => accept new client
    ** - readable client fd => recv and parse lines
    ** - writable client fd with pending outbuf => flush queued output
    */
    while (1)
    {
        readfds = all;
        writefds = all;

        if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0)
            ft_err(NULL);

        for (fd = 0; fd <= max_fd; fd++)
        {
            if (!FD_ISSET(fd, &readfds))
                continue;

            if (fd == sockfd)
            {
                alen = sizeof(addr);
                cfd = accept(sockfd, (struct sockaddr *)&addr, &alen);
                if (cfd >= 0)
                    add_client(cfd);
            }
            else
                handle_read(fd);
        }

        for (fd = 0; fd <= max_fd; fd++)
        {
            if (fd != sockfd && FD_ISSET(fd, &writefds) && outbuf[fd])
                flush_client(fd);
        }
    }
    return 0;
}