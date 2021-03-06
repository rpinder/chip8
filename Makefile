CPPFLAGS=-Wall -pedantic -Wextra -std=c++17

chip8: cpu.o memory.o renderer.o util.o main.cpp
	g++ $(CPPFLAGS) -o chip8 main.cpp cpu.o memory.o renderer.o util.o -lSDL2

cpu.o: cpu.cpp cpu.hpp memory.hpp renderer.hpp
	g++ $(CPPFLAGS) -c cpu.cpp

memory.o: memory.cpp memory.hpp
	g++ $(CPPFLAGS) -c memory.cpp

renderer.o: renderer.cpp renderer.hpp
	g++ $(CPPFLAGS) -c renderer.cpp -lSDL2

util.o: util.cpp
	g++ $(CPPFLAGS) -c util.cpp

