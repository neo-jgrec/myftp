##
## EPITECH PROJECT, 2024
## tcp_lib
## File description:
## Makefile
##

PROJECT_NAME			=	tcp_lib
NAME					=	lib$(PROJECT_NAME).a

SRC 					:= src/accept.c	        \
                           src/connect.c		\
						   src/fd.c				\
						   src/listen.c			\
						   src/recv.c			\
						   src/select.c			\
						   src/send.c

BUILD_DIR 				= 	../build/libtcp

BUILD_DIR_DEFAULT		=	$(BUILD_DIR)/default
OBJ						=	$(SRC:%.c=$(BUILD_DIR_DEFAULT)/%.o)

DEBUG_BUILD_DIR			=	$(BUILD_DIR)/debug
DEBUG_OBJ				=	$(SRC:%.c=$(DEBUG_BUILD_DIR)/%.o)

TEST_BUILD_DIR			=	$(BUILD_DIR)/tests
TEST_OBJ				=	$(SRC:%.c=$(TEST_BUILD_DIR)/%.o)\
$(TEST_SRC:%.c=$(TEST_BUILD_DIR)/%.o)

CRITERION				=	-lcriterion --coverage
LCOV 					:= 	-fprofile-arcs -ftest-coverage
TEST_NAME				=	unit_tests

CFLAGS					=	-I include/ -I libtcp/include/
WFLAGS					=	-Wall -Wextra

_default:
	@$(MAKE) -j all --no-print-directory

all:	$(NAME)

$(NAME):	$(OBJ)
	@ar rc $(NAME) $(OBJ)\
	&& echo -e "\033[1;32m[OK]\033[0m" $(NAME)\
	|| echo -e "\033[1;31m[KO]\033[0m" $(NAME)\
	&& ranlib $(NAME)

$(BUILD_DIR_DEFAULT)/%.o:	%.c
	@mkdir -p $(dir $@)
	@gcc -c -o $@ $< $(CFLAGS) $(WFLAGS)\
	&& echo -e "\033[1;32m[OK]\033[0m" $<\
	|| echo -e "\033[1;31m[KO]\033[0m" $<

_clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(shell find -name '*.gc*' -type f)

clean: _clean
	@echo -e "\033[1;31m[CLEAN]\033[0m" $(PROJECT_NAME)

fclean: _clean
	@echo -e "\033[1;31m[FCLEAN]\033[0m" $(PROJECT_NAME)
	@rm -f $(NAME)
	@rm -f $(TEST_NAME)

re:	fclean _default

debug:	CFLAGS += -g -DDEBUG
debug:	$(DEBUG_OBJ)
	@ar rc $(NAME) $(DEBUG_OBJ)\
	&& echo -e "\033[1;32m[OK]\033[0m" $(NAME)\
	|| echo -e "\033[1;31m[KO]\033[0m" $(NAME)\
	&& ranlib $(NAME)

$(DEBUG_BUILD_DIR)/%.o:	%.c
	@mkdir -p $(dir $@)
	@gcc -c -o $@ $< $(CFLAGS) $(WFLAGS)\
	&& echo -e "\033[1;32m[OK]\033[0m" $<\
	|| echo -e "\033[1;31m[KO]\033[0m" $<

tests_run:	CFLAGS += $(CRITERION)
tests_run:	$(TEST_NAME)
	@./$(TEST_NAME) --verbose --always-succeed
	gcovr --exclude tests/

$(TEST_NAME):	$(TEST_OBJ)
	@gcc -o $(TEST_NAME) $(TEST_OBJ) $(CRITERION) $(CFLAGS)\
	&& echo -e "\033[1;32m[OK]\033[0m" $(TEST_NAME)\
	|| echo -e "\033[1;31m[KO]\033[0m" $(TEST_NAME)

$(TEST_BUILD_DIR)/%.o:	%.c
	@mkdir -p $(dir $@)
	@gcc -c -o $@ $< $(CFLAGS) $(WFLAGS)\
	&& echo -e "\033[1;32m[OK]\033[0m" $<\
	|| echo -e "\033[1;31m[KO]\033[0m" $<

.PHONY: _clean clean _default fclean re debug tests_run

.SILENT: _clean clean _default fclean re debug tests_run
