#Name
NAME= minishell

#COMPILING
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf

VFLAGS= -s --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose\
 		--gen-suppressions=all --suppressions=valgrind_leaks_ignore.txt #--trace-children=yes

ifdef DEBUG
	CFLAGS += -g3
endif

ifdef LOG
	VFLAGS += --log-file=memleaks.log
endif

ifdef SIL
	VFLAGS += --child-silent-after-fork=yes
endif

#DIRS
LIBFT_DIR= ./libft
INCS= ./include
SRCS_DIR= ./src
OBJS_DIR= ./obj

#LIBS
LIBFT= ${LIBFT_DIR}/libft.a
LIBFT_LIB= -L${LIBFT_DIR} -lft
LIBS= ${LIBFT_LIB} -lreadline

GRAY	=	\e[33;2;37m
RESET	=	\e[0m
CURSIVE	=	\e[33;3m

#FILES
SRCF= cd echo env exec_cmd executor executor_utils exit expansor export \
	export_utils lexer lexer_utils parser parser_utils pipes redirects signals \
	split_to_tokens unset utils redirect_utils

SRCS= $(addprefix ${SRCS_DIR}/, $(addsuffix .c, ${SRCF}))
OBJS= $(addprefix ${OBJS_DIR}/, $(addsuffix .o, ${SRCF}))

all: ${NAME}

${NAME}: ${LIBFT} ${OBJS}
	${CC} ${CFLAGS} main.c ${OBJS} -I${INCS} ${LIBS} -o ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c | ${OBJS_DIR}
	${CC} ${CFLAGS} -I${INCS} -c $< -o $@

${OBJS_DIR}:
	mkdir -p ${OBJS_DIR}

${LIBFT}:
	make bonus -sC ${LIBFT_DIR}

bonus:

clean:
	${RM} ${OBJS_DIR}
	make clean -sC ${LIBFT_DIR}

fclean: clean
	${RM} ${NAME}
	make fclean -sC ${LIBFT_DIR}

re: fclean all

debug:
	make DEBUG=1 bonus -sC ${LIBFT_DIR}
	${CC} -g3 main.c ${SRCS} -I${INCS} ${LIBS} -o ${NAME} && gdb --tui ./minishell

mem: re
	make -s clean
	valgrind ${VFLAGS} ./minishell

run: re
	make -s clean
	./minishell

norm:
	@printf "$(CURSIVE)$(GRAY)"
	@norminette | grep -v OK!
	@printf "$(RESET)"

.PHONY: all clean fclean re
