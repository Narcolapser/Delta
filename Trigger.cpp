#include "Trigger.h"

Trigger::Trigger(xml_node val)
{
	arg = val;
	parentType = stringToEnum(val.parent().name());
	globalRM->AssignID(globalRM->RequestID(),this);
}
Trigger::~Trigger()
{
	globalRM->Release(ID);
}
void Trigger::trip()
{
	switch(parentType)
	{
		case OBJECT:
			parent->trip(arg);
			break;
//		case BUFFER:
		case CAMERA:
			((Camera*)parent)->trip(arg);
			break;
		case GEOOBJECT:
			((GeoObject*)parent)->trip(arg);
			break;
//		case MESH:
		case MODEL:
			((Model*)parent)->trip(arg);
			break;
//		case PROGRAM:
		default: break;
	}
}
void Trigger::setParent(Object* val)
{
	parent = val;
}
//	Object* parent;
//	delta_t parentType;
//	xml_node arg;

