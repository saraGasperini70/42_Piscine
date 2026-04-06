NAME = bsq

CMD = gcc -Wall -Wextra -Werror -g

SRCS = main.c \

all: ${NAME}

${NAME}:
	@${CMD} ${SRCS} -o ${NAME}

clean:
	@rm -rf *.o ${NAME}

re: clean all
