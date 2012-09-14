#include "Event.h"
#ifndef EVENT_CPP
#define EVENT_CPP

Event::Event(UID _sender, event_t _type)
{
	sender = sender;
	receiver = (UID)0;
	type = _type;
	args[0] = Variant();
	args[1] = Variant();
	args[2] = Variant();
	args[3] = Variant();
}
Event::Event()
{
	sender = (UID)0;
	receiver = (UID)0;
	type = NOT_EVENT;
	args[0] = Variant();
	args[1] = Variant();
	args[2] = Variant();
	args[3] = Variant();
}
Event::Event(const Event& event)
{
	sender = event.sender;
	receiver = event.receiver;
	type = event.type;
	args[0] = event.args[0];
	args[1] = event.args[1];
	args[2] = event.args[2];
	args[3] = event.args[3];
}
Event::~Event()
{
	sender = 0;
	type = NOT_EVENT;
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
bool Event::setArgs(int arg, char* val, int len)
{
	for(int i = 0; i < len; i++)
	{
		args[arg].datum.v_asChar[i] = val[i];
	}
}

/*.S.D.G.*/
#endif
//int main(){return 0;}
