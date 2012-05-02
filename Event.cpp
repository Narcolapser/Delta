#include "Event.h"
#ifndef EVENT_CPP
#define EVENT_CPP

Event::Event(UID _sender, event_t _type)
{
	sender = sender;
	type = _type;
	args[0] = Variant();
	args[1] = Variant();
	args[2] = Variant();
	args[3] = Variant();
}
Event::~Event()
{
	sender = 0;
	type = NOT_EVENT;
//	char clear = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',};
//	args[0] = Variant(clear);
//	args[1] = Variant(clear);
//	args[2] = Variant(clear);
//	args[3] = Variant(clear);
	args[0] = Variant();
	args[1] = Variant();
	args[2] = Variant();
	args[3] = Variant();
}
bool Event::setArg(Variant arg)
{
	args[0] = arg;
}
bool Event::setArgs(Variant arg0, Variant arg1, Variant arg2, Variant arg3)
{
	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;
}
bool Event::setArgs(int val0, int val1, int val2, int val3)
{
	args[0] = Variant(val0,val1,val2,val3);
}
bool Event::setArgs(float val0, float val1, float val2, float val3)
{
	args[0] = Variant(val0,val1,val2,val3);
}

/*.S.D.G.*/
#endif
//int main(){return 0;}
