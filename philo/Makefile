SRCS = main.c \
		parameters.c \
		utils.c \
		errors.c \
		create_threads.c

all:
	gcc -fsanitize=thread -g -Wall -Wextra -Werror -g -pthread $(SRCS)
