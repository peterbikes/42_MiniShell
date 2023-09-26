NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

ifdef DEBUG
	CFLAGS += -g3
endif

#Directories
OBJ_DIR = objs/

#Source files
SRCF = ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint\
	  ft_strlen ft_memset ft_bzero ft_memcpy ft_memmove ft_strlcpy\
	  ft_strlcat ft_toupper ft_tolower ft_strchr ft_strrchr\
	  ft_strncmp ft_memchr ft_memcmp ft_strnstr ft_atoi ft_calloc\
	  ft_strdup ft_substr ft_strjoin ft_strtrim ft_split ft_itoa\
	  ft_strmapi ft_striteri ft_putchar_fd ft_putstr_fd\
	  ft_putendl_fd ft_putnbr_fd get_next_line ft_nbr_utils ft_print_chars\
	  ft_printf ft_print_nbrs ft_print_ptr ft_db_free ft_free ft_atol ft_atoll

SRCF_BONUS = ft_lstnew_bonus ft_lstadd_front_bonus ft_lstsize_bonus\
			ft_lstlast_bonus ft_lstadd_back_bonus ft_lstdelone_bonus\
			ft_lstclear_bonus	ft_lstiter_bonus ft_lstmap_bonus

SRC = $(addsuffix .c, ${SRCF})
OBJ = $(addprefix ${OBJ_DIR}, $(addsuffix .o, ${SRCF}))
OBJ_BONUS = $(addprefix ${OBJ_DIR}, $(addsuffix .o, ${SRCF_BONUS}))

#OBJ = ${SRCF:.c=.o}
#OBJ_BONUS = ${SRCF_BONUS:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	ar rcs ${NAME} ${OBJ}

${OBJ_DIR}%.o: %.c | ${OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}


bonus: ${NAME} ${OBJ_BONUS}
	ar rcs ${NAME} ${OBJ_BONUS}

re: fclean all

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -f ${NAME}

.PHONY: all clean fclean re bonus
