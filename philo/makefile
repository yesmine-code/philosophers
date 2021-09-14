NAME	 	= philo

SRCS		=  lib.c main.c list.c

INCLUDES	=	include

OBJS		= ${SRCS:.c=.o}

OBJS		= ${SRCS:.c=.o}

CC			= gcc

RM			= rm -f

CFLAGS		= -Wall -Wextra -Werror -pthread

.c.o:
				${CC} ${CFLAGS} -c -I ${INCLUDES} $< -o ${<:.c=.o} 


all: 			${NAME}

${NAME}:	$(OBJS) 
			@$(CC) $(CFLAGS) -g -I $(INCLUDES) $(OBJS) -o $(NAME)


clean:
				${RM} ${OBJS} 

fclean:			clean
				${RM} ${NAME}

re:				fclean all

.PHONY:			all clean fclean re