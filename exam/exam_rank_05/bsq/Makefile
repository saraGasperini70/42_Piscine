NAME = bsq

SRCS = main.c

FLAGS = gcc -Wall -Wextra -Werror

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}:
	${FLAGS} ${SRCS} -o ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re
