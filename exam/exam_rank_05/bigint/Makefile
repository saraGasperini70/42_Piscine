NAME = bigint

SRC = bigint.cpp \
	main.cpp \

OBJ = ${SRC:.cpp=.o}

FLAGS = c++ -Wall -Wextra -Werror -g -std=c++98

all: ${NAME}

${NAME}:
	@clear && ${FLAGS} ${SRC} -o ${NAME}

clean:
	@rm -rf *.o

fclean: clean
	@rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re