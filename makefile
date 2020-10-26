console.o: chessprog.h console.h
	gcc -c console.c -o console.o

gui.o: chessprog.h gui.h
	gcc -c gui.c -o gui.o -I/usr/local/include/SDL

game: console.o gui.o chessprog.h
	gcc chessprog.c -o game console.o gui.o -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE -L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa

clean:
	rm -rf *.o game
