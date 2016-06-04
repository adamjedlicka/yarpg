
program = yarpg

CC = g++
CF = -std=c++11 -Wall -pedantic
CL = -lncurses

src = src/main.cpp src/engine.cpp src/map.cpp src/player.cpp
obj = build/main.o build/engine.o build/map.o build/player.o

build: ${program}
	
clean:
	rm -rf build
	rm -rf bin

install: bin/${program}
	cp -f bin/${program} /usr/bin/${program}
	
uninstall:
	rm -f /usr/bin/${program}

${program}: ${obj}
	mkdir -p bin
	cp -rf src/data bin
	${CC} ${obj} ${CL} -o bin/${program}

${obj}: ${src}
	mkdir -p build
	cd build
	${CC} ${CF} ${CL} -c ${src}
	mv *.o build/
