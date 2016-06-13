
program = jedliad1

CF = -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb
CL = -lncurses
CD = #-g -fsanitize=address

all: ${program}

compile: ${program}

run: compile
	./${program}

doc:
	
clean:
	rm -rf build
	rm -rf ${program}

${program}:
	mkdir -p build
	cd build
	g++ ${CF} ${CD} ${CL} -c src/*.cpp
	mv *.o build/
	g++ build/*.o ${CD} ${CL} -o ${program}
