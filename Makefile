CC			= cc
CFLAGS		= -Wall -Werror -Wextra -g
NAME		= pipex
BNAME		= bonus_pipex

SRCS		=	pipex	\
				errors	\
				utils	\

SRCS_BONUS	=	pipex_cmds_bonus	\
				pipex_exec_bonus	\
				errors		\
				utils		\


INCS		=	-I libft \
				-I get_next_line \

SRC			= $(addsuffix .c, $(SRCS))
OBJ			= $(addsuffix .o, $(SRCS))
SRC_B		= $(addsuffix .c, $(SRCS_BONUS))
OBJ_B		= $(addsuffix .o, $(SRCS_BONUS))


all: $(NAME)

$(NAME) : $(OBJ)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) -o $@ $(OBJ) libft/libft.a

bonus: $(BNAME)

$(BNAME) : $(OBJ_B)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) -o $@ $(OBJ_B) libft/libft.a

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)

clean:
	$(MAKE) -C libft/ clean
	rm -rf $(OBJ) $(OBJ_B)

fclean: clean
	$(MAKE) -C libft/ fclean
	rm -f $(NAME) $(BNAME)

re: fclean all

.PHONY: all clean fclean re