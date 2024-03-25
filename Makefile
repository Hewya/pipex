CC			= gcc
CFLAGS		= -Wall -Werror -Wextra
NAME		= pipex
BNAME		= bonus_pipex

SRCS		=	pipex	\
				errors	\
				parsing	\

SRCS_BONUS	=	pipex_bonus	\
				errors		\
				parsing		\

INCS		= -I libft
		
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