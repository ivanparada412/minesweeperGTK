minas.out: minas.o modulo1.o modulo2.o
	gcc -Wall -o minas.out minas.o modulo1.o modulo2.o `pkg-config --libs gtk+-2.0`

minas.o: minas.c
	gcc -c -g minas.c `pkg-config --cflags gtk+-2.0`

modulo1.o: modulo1.c
	gcc -c -g modulo1.c  `pkg-config --cflags gtk+-2.0`

modulo2.o: modulo2.c
	gcc -c -g modulo2.c `pkg-config --cflags gtk+-2.0`

clean:
	rm *.dat

