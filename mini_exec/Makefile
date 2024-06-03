# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 16:33:46 by aguezzi           #+#    #+#              #
#    Updated: 2024/06/02 17:25:30 by aguezzi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

SRCS	= minishell.c create_lists.c tokens_error.c cmds_args.c infile_outfile.c heredocs.c debugg.c

CC	= gcc

FLAGS	= -Wall -Wextra -Werror

OBJS	= ${SRCS:.c=.o}

RM	= rm -f

all:	${NAME}

${NAME}:	${OBJS}
		@make -C libft
		@${CC} ${FLAGS} ${OBJS} libft/libft.a -o ${NAME}

bonus:		${OBJS_BONUS}
		@make -C libft
		@${CC} ${FLAGS} ${OBJS_BONUS} libft/libft.a -o ${NAME_BONUS}

clean:
		@echo "clean all..."
		@${RM} ${OBJS} ${OBJS_BONUS}
		@make clean -C libft

fclean:	clean
		@echo "fclean all..."
		@${RM} ${NAME} ${NAME_BONUS}
		@${RM} libft/libft.a

re:	fclean all

.PHONY:	all clean fclean re