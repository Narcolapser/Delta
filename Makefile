#CC=g++
#LDLIBS=-lglut -lGLEW -lGL -lm
#all: Delta

Delta : Attrib.o Mesh.o Camera.o Program.o Buffer.h
	g++ Delta.cpp -o Delta -lglut -lGLEW -lGL -lm
Mesh.o : Attrib.o
	g++ -c Mesh.h -lglut -lGLEW -lGL -lm
Attrib.o : Program.o Buffer.o
	g++ -c Attrib.h -lglut -lGLEW -lGL -lm
Program.o :
	g++ -c Program.h -lglut -lGLEW -lGL -lm
Camera.o :
	g++ -c Camera.h -lm
Buffer.o :
	g++ -c Buffer.h -lglut -lGlew -lGL -lm

#http://www.eng.hawaii.edu/Tutor/Make/
