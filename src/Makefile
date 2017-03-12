all: slicyl

slicyl: main.o slicer.o Triangle.o TriangleMesh.o
	g++ -Wall -o slicyl main.o slicer.o Triangle.o TriangleMesh.o

main.o: main.cpp dimensional_space.h
	g++ -Wall -c main.cpp dimensional_space.h

slicer.o: slicer.cpp slicer.h
	g++ -Wall -c slicer.cpp
    
Triangle.o: Triangle.cpp Triangle.h
	g++ -Wall -c Triangle.cpp

TriangleMesh.o: TriangleMesh.cpp TriangleMesh.h
	g++ -Wall -c TriangleMesh.cpp