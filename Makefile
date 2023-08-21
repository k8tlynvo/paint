paint.out: main.o setup.o command_validation.o paint.o
	gcc -g -Wall -Werror -o paint.out main.o setup.o command_validation.o paint.o

main.o: main.c setup.h paint.h
	gcc -g -Wall -Werror -c main.c

setup.o: setup.c setup.h
	gcc -g -Wall -Werror -c setup.c

command_validation.o: command_validation.c command_validation.h setup.h paint.h
	gcc -g -Wall -Werror -c command_validation.c

paint.o: paint.c paint.h command_validation.h
	gcc -g -Wall -Werror -c paint.c

clean:
	rm -fr *.o *.out
