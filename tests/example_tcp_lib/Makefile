##
## EPITECH PROJECT, 2024
## tcp_lib
## File description:
## Makefile
##

all:
	gcc client.c -o client -lpthread -I\
	../../include -L ../../ -l tcp_lib -Wall -Wextra -I include
	gcc server.c -o server -lpthread -I\
	../../include -L ../../ -l tcp_lib -Wall -Wextra -I include
	echo -e "\033[1;32m[OK]\033[0m CHAT EXAMPLE"

debug:	CFLAGS += -g -DDEBUG
debug:
	gcc client.c -o client -lpthread -DDEBUG -I\
	../../include -L ../../ -l tcp_lib -Wall -Wextra -I include
	gcc server.c -o server -lpthread -DDEBUG -I\
	../../include -L ../../ -l tcp_lib -Wall -Wextra -I include
	echo -e "\033[1;32m[OK]\033[0m CHAT EXAMPLE"

clean:
	@echo -e "\033[1;31m[CLEAN]\033[0m" $(PROJECT_NAME)
	@rm -f client server

fclean: clean
	@echo -e "\033[1;31m[FCLEAN]\033[0m" $(PROJECT_NAME)
	@rm -f client server

re:	fclean all

.PHONY: all clean fclean re debug tests_run

.SILENT: all clean fclean re debug tests_run

%:
	@echo -e "\033[1;31m[ERROR]\033[0m" "No rule to make target '$@'"
