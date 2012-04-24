#CC=g++
#LDLIBS=-lglut -lGLEW -lGL -lm
#all: Delta

Delta : Camera.o ResourceManager.o Scene.o Delta.cpp
	g++ Delta.cpp -o Delta -lglut -lGLEW -lGL -lm
ResourceManager.o : Object.o Buffer.o Mesh.o pugixml.o ResourceManager.cpp ResourceManager.h
	g++ -c ResourceManager.h -lm
Mesh.o : GeoObject.o Attrib.o Mesh.h Mesh.cpp
	g++ -c Mesh.h Mesh.cpp -lglut -lGLEW -lGL -lm
Attrib.o : Program.o Buffer.o Attrib.h Attrib.cpp
	g++ -c Attrib.h Attrib.cpp -lglut -lGLEW -lGL -lm
Camera.o : GeoObject.o ResourceManager.o Camera.h
	g++ -c Camera.h -lm
Scene.o : GeoObject.o Camera.o Model.h ResourceManager.o Trigger.o Scene.h Scene.cpp
	g++ -c Scene.h Scene.cpp -lm
Buffer.o : Object.o Buffer.h Buffer.cpp
	g++ -c Buffer.h Buffer.cpp -lglut -lGlew -lGL -lm
GeoObject.o : Object.o GeoObject.h
	g++ -c GeoObject.h -lm
Program.o : Program.h Program.cpp
	g++ -c Program.h Program.cpp -lglut -lGLEW -lGL -lm
Object.o : Object.h Object.cpp
	g++ -c Object.h Object.cpp -lm
pugixml.o: lib/pugixml/pugixml.hpp
	g++ -c lib/pugixml/pugixml.hpp -lm
Trigger.o: Object.o ResourceManager.o Model.o Camera.o GeoObject.o Program.o Trigger.h Trigger.cpp
	g++ -c Trigger.h Trigger.cpp -lm
Model.o: ResourceManager.o Mesh.o GeoObject.o Program.o Model.h
	g++ -c Model.h -lm

clean :
	rm *.gch *~ Delta ./lib/*/*.gch ./DevDocs/*~ *.o

#http://www.eng.hawaii.edu/Tutor/Make/
