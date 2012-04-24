/* This class wraps all necessary features for making a data defined user interface.*/

#ifndef TRIGGER_H
#define TRIGGER_H

#include <vector>

#include "Object.cpp"
#include "ResourceManager.cpp"
#include "Model.h"
#include "Camera.h"
#include "GeoObject.cpp"
#include "Program.cpp"

typedef void arg;

using namespace std;

class Trigger: public Object
{
public:
	Trigger(xml_node val);
	~Trigger();
	void trip();
	void setParent(Object* val);
private:
	Object* parent;
	delta_t parentType;
	xml_node arg;
};

/*.S.D.G.*/
#endif
