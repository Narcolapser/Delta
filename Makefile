#CC=g++
#LDLIBS=-lglut -lGLEW -lGL -lm
#all: Delta

Delta : Camera.o ResourceManager.o Scene.o Delta.cpp
	g++ Delta.cpp -o Delta -lglut -lGLEW -lGL -lm
ResourceManager.o : Object.o Buffer.o Mesh.o pugixml.o ResourceManager.cpp ResourceManager.h
	g++ -c ResourceManager.h -lm
Mesh.o : GeoObject.o Attrib.o Mesh.h Mesh.cpp
	g++ -c Mesh.h -lglut -lGLEW -lGL -lm
Attrib.o : Program.o Buffer.o Attrib.h
	g++ -c Attrib.h -lglut -lGLEW -lGL -lm
Camera.o : GeoObject.o ResourceManager.o Camera.h
	g++ -c Camera.h -lm
Scene.o : GeoObject.o Camera.o Model.h ResourceManager.o Scene.h Scene.cpp
	g++ -c Scene.h -lm
Buffer.o : Object.o Buffer.h
	g++ -c Buffer.h -lglut -lGlew -lGL -lm
GeoObject.o : Object.o GeoObject.h
	g++ -c GeoObject.h -lm
Program.o : Program.h Program.cpp
	g++ -c Program.h -lglut -lGLEW -lGL -lm
Object.o : Object.h
	g++ -c Object.h -lm
pugixml.o: lib/pugixml/pugixml.hpp
	g++ -c lib/pugixml/pugixml.hpp -lm

clean :
	rm *.gch *~ Delta ./lib/*/*.gch ./DevDocs/*~

#http://www.eng.hawaii.edu/Tutor/Make/
