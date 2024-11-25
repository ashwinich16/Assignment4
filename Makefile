# Makefile for compiling the program

# Default target
all: my_program

# Linking all object files into the final executable
my_program: main.o BibParser.o publication.o 
	g++ main.o BibParser.o publication.o -o my_program

# Object file targets
main.o: main.cpp
	g++ -c main.cpp

BibParser.o: BibParser.cpp
	g++ -c BibParser.cpp
	
publication.o: publication.cpp
	g++ -c publication.cpp

# Clean target
clean:
	rm -f *.o my_program
