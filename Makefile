#CC=g++
#LDLIBS=-lglut -lGLEW -lGL -lm
#all: Delta

Delta : ResourceManager.o Scene.o Interface.o Delta.cpp
	g++ Delta.cpp -o Delta -lglut -lGLEW -lGL -lm
	rm *.gch
ResourceManager.o : Object.o Buffer.o Mesh.o pugixml.o Interface.o ResourceManager.cpp ResourceManager.h
	g++ -c ResourceManager.h ResourceManager.cpp -lm

Camera.o : GeoObject.o ResourceManager.o Camera.h Camera.cpp
	g++ -c Camera.h Camera.cpp -lm

Scene.o : GeoObject.o Camera.o Model.o ResourceManager.o Scene.h Scene.cpp
	g++ -c Scene.h Scene.cpp -lm

Model.o: ResourceManager.o Mesh.o GeoObject.o Program.o Model.h Model.cpp
	g++ -c Model.h Model.cpp -lm

GeoObject.o : Object.o GeoObject.h GeoObject.cpp
	g++ -c GeoObject.h GeoObject.cpp -lm

Mesh.o : Attrib.o Mesh.h Mesh.cpp
	g++ -c Mesh.h Mesh.cpp -lglut -lGLEW -lGL -lm

Buffer.o : Object.o Buffer.h Buffer.cpp
	g++ -c Buffer.h Buffer.cpp -lglut -lGlew -lGL -lm

Attrib.o : Program.o Buffer.o Attrib.h Attrib.cpp
	g++ -c Attrib.h Attrib.cpp -lglut -lGLEW -lGL -lm

Program.o : Program.h Program.cpp
	g++ -c Program.h Program.cpp -lglut -lGLEW -lGL -lm

Object.o : Object.h Object.cpp Event.o pugixml.o Lib.h Lib.cpp
	g++ -c Object.h Object.cpp -lm

pugixml.o: lib/pugixml/pugixml.hpp lib/pugixml/pugixml.cpp
	g++ -c lib/pugixml/pugixml.hpp lib/pugixml/pugixml.cpp -lm

Interface.o : Interface.h Interface.cpp Lib.h Lib.cpp
	g++ -c Interface.h Interface.cpp -lm

Event.o: Event.h Event.cpp
	g++ -c Event.h Event.cpp -lm

clean :
	rm *~ Delta ./lib/*/*.o ./DevDocs/*~ *.o ./lib/*/*.gch

#http://www.eng.hawaii.edu/Tutor/Make/
