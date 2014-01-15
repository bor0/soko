@all:
	@gcc main.c src/*.c -I/usr/include/SDL/ -Iinclude/ -lSDL_image -lSDL_mixer -lSDL -o soko -ansi

clean:
	@rm -f soko soko.tar.gz

deps:
	@sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev

package:
	make
	tar -czvf soko.tar.gz soko gfx music levels
