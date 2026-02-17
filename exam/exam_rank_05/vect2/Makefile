NAME = vect2

SRC = 	main.cpp \
		vect2.cpp \

OBJS = ${SRC:.cpp=.o}

FLAGS = c++ -Wextra -Wall -Werror -g -std=c++98

all: ${NAME}
	
${NAME}:
	@${FLAGS} ${SRC} -o ${NAME}

clean:
	@rm -rf *.o

fclean: clean
	@rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re