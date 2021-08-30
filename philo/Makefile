SRCS = main.c \
		parameters.c \
		utils.c \
		errors.c \
		threads_and_mutexes.c \
		simulation.c

NAME = philo

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror

$(NAME): $(OBJS)
	gcc $(OBJS) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all