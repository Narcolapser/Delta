/* This class is the master ancestor class. Anything that is brought into existance by the resource
 * manager class. The resource manager will maintain UUIDs that can be used for global reference of
 * any object. so this includes Meshes, materials, cameras, buffers, etc. It is worth noteing that
 * some objects may have more than one object in them. any time there is an object that has a HAS-A
 * relationship with another object, then it will have sub objects. some IS-A relationships will
 * also have subobjects. I can't actually think of an example of that right now, so I'm going to
 * for now assume that the only something can have a subobject is in a HAS-A relationship, like how
 * a mesh HAS-A material. 
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include "lib/pugixml/pugixml.cpp"

using namespace std;

typedef int UID;

enum delta_t {FAIL,OBJECT,BUFFER,CAMERA,GEOOBJECT,MESH,MODEL,SCENE,PROGRAM,TRIGGER};

class Object
{
public:
	Object();
	virtual ~Object();
	UID getID();
	void trip(xml_node arg);
protected:
	UID ID;
	//delta_t type;
};

#endif
/*
class Object
{
public:
	Object()
	{
		ID = 0;
	}
	virtual ~Object()
	{
		ID = 0;
	}
	UID getID()
	{
		return ID;
	}
	void trip(xml_node arg)
	{//there really isn't anything that an object can do...
		return;
	}
protected:
	UID ID;
	//delta_t type;
};


/*.S.D.G.*/

//#endif
