/* This class wraps all necessary features for making a data defined user interface.*/

#ifndef TRIGGER_H
#define TRIGGER_H

#include <vector>

#include "Object.h"
#include "ResourceManager.cpp"
#include "Model.h"
#include "Camera.h"
#include "GeoObject.h"
#include "Program.h"

typedef void arg;

using namespace std;

class Trigger: public Object
{
public:
	Trigger(xml_node);
	~Trigger();
	void trip();
private:
	Object* parent;
	delta_t parentType;
	void (*callFunc)(void);
	vector<arg*> args;
};

/*.S.D.G.*/
#endif
