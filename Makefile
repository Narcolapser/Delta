#CC=g++
#LDLIBS=-lglut -lGLEW -lGL -lm
#all: Delta

Delta : Attrib.o Mesh.o Camera.o Program.o Buffer.o ResourceManager.o Scene.o
	g++ Delta.cpp -o Delta -lglut -lGLEW -lGL -lm
ResourceManager.o : Object.o GeoObject.o Program.o Camera.o Buffer.o Mesh.o pugixml.o
	g++ -c ResourceManager.h -lm
Mesh.o : GeoObject.o Attrib.o
	g++ -c Mesh.h -lglut -lGLEW -lGL -lm
Attrib.o : Program.o Buffer.o
	g++ -c Attrib.h -lglut -lGLEW -lGL -lm
Camera.o : GeoObject.o
	g++ -c Camera.h -lm
Scene.o : GeoObject.o Mesh.o Camera.o
	g++ -c Scene.h -lm
Buffer.o : GeoObject.o
	g++ -c Buffer.h -lglut -lGlew -lGL -lm
GeoObject.o :
	g++ -c GeoObject.h -lm
Program.o :
	g++ -c Program.h -lglut -lGLEW -lGL -lm
Object.o :
	g++ -c Object.h -lm
pugixml.o:
	g++ -c lib/pugixml/pugixml.hpp -lm


#http://www.eng.hawaii.edu/Tutor/Make/
