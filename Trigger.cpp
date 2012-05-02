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
void Trigger::setParent(Object* val)
{
	parent = val;
}
//	Object* parent;
//	delta_t parentType;
//	xml_node arg;

