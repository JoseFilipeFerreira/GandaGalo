CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2
FICHEIROS= parser.h cgi.h frontend.c frontend.h frontendTab.c frontendTab.h userfiles.c userfiles.h givehelp.c solver.c givehelp.h decide.c decide.h state.c state.h stack.c stack.h validate.c validate.h filemanager.c filemanager.h estado.c estado.h exemplo.c Makefile
RANDOMFILES= solver.h
EXECUTAVEL=GandaGalo
RANDOMEXE=gerar

install: $(EXECUTAVEL)
	sudo cp $(EXECUTAVEL) /usr/lib/cgi-bin

	sudo mkdir -p /var/www/html/images
	sudo cp ./images/*.png /var/www/html/images

	sudo mkdir -p /var/www/html/ficheiro
	sudo mkdir -p /var/www/html/ficheiro/mapas
	sudo mkdir -p /var/www/html/ficheiro/mapas/random
	sudo mkdir -p /var/www/html/ficheiro/users
	sudo mkdir -p /usr/local/games/GandaGalo
	sudo mkdir -p /usr/local/games/GandaGalo/users

	sudo cp ./files/*.save /var/www/html/ficheiro
	sudo cp ./files/*.map /var/www/html/ficheiro/mapas
	sudo chmod -R a+rwx /usr/local/games/GandaGalo/*
	sudo chmod -R a+rwx /var/www/html/ficheiro/*

	touch install

$(EXECUTAVEL): leaderboard.o parser.o frontend.o frontendTab.o exemplo.o estado.o solver.o validate.o state.o stack.o filemanager.o decide.o givehelp.o userfiles.o
	cc -o $(EXECUTAVEL) leaderboard.o parser.o frontend.o frontendTab.o exemplo.o estado.o solver.o validate.o state.o stack.o filemanager.o decide.o givehelp.o userfiles.o

random: gerar.o solver.o estado.o state.o stack.o validate.o leaderboard.o
	cc -o $(RANDOMEXE) gerar.o solver.o estado.o state.o stack.o validate.o leaderboard.o

imagens:
	sudo mkdir -p /var/www/html/images
	sudo cp ./images/*.png /var/www/html/images

ficheiros:
	sudo mkdir -p /var/www/html/ficheiro
	sudo cp ./files/*.save ./files/*.map /var/www/html/ficheiro

GandaGalo.zip: $(FICHEIROS)
	zip -9 GandaGalo.zip $(FICHEIROS)

doc:
	doxygen

clean:
	rm -rf *.o $(EXECUTAVEL) $(RANDOMEXE) latex html install

estado.o: estado.c estado.h stack.c stack.h state.h decide.h frontend.h
frontendTab.o: frontend.h
stack.o: stack.c stack.h userfiles.h
validate.o: estado.h validate.c validate.h
exemplo.o: exemplo.c frontend.h cgi.h estado.h validate.h 
filemanager.o: filemanager.c filemanager.h estado.c estado.h
solver.o: solver.c estado.c estado.h
userfiles.o: userfiles.h stack.h estado.h
