all: slicyl

slicyl: main.o Slicer.o Triangle.o TriangleMesh.o SlicedLayers.o
	g++ -Wall -o $@ main.o Slicer.o Triangle.o TriangleMesh.o SlicedLayers.o

main.o: main.cpp dimensional_space.h
	g++ -Wall -o $@ -c main.cpp 

Slicer.o: Slicer.cpp Slicer.h
	g++ -Wall -o $@ -c Slicer.cpp
    
Triangle.o: Triangle.cpp Triangle.h
	g++ -Wall -o $@ -c Triangle.cpp

TriangleMesh.o: TriangleMesh.cpp TriangleMesh.h
	g++ -Wall -o $@ -c TriangleMesh.cpp
	
SlicedLayers.o: SlicedLayers.cpp SlicedLayers.h
	g++ -Wall -o $@ -c SlicedLayers.cpp