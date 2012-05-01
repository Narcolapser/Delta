/*This class exists to work as a message passing system between objects and subsystems of the game.
 *It will wrap both the information that is to be sent and the type of event it is.
 */

#ifndef EVENT_H
#define EVENT_H
#define ARG_SIZE 16 //can't be less than 8.
#define ARG_COUNT 4 //can't be less than 1.

#include <stdlib.h>

enum event_t 
{
	NOT_EVENT,//something to clear to.
	EVENT,//also known as a trip event.
		//Arg type: bool. 
		//Args contain: first in the first arg is a boolean tripped or not.
		//Applicable: Really anything.
	EVENT_MOVE,//a simple move command.
		//Arg type: float.
		//Args contain: first 3 of the first contain the x,y, and z to move.
		//Applicable: GeoObjects.
	EVENT_ROTATE,//another one only useful to GeoObjects. Rotate.
		//Arg type: float.
		//Args contain: first 3 of first contain the x,y, and z to rotate about.
		//Applicable: GeoObjects.
	EVENT_SET_LOC,//a command to move to this exact location
		//Arg Type: float.
		//Args contain: first 3 of first contain the x,y,z of new location.
		//Applicable: GeoObjects.
	EVENT_SET_ROTATION,//A command to set an object to this exact rotation.
		//Arg Type: float.
		//Args contain: first 3 of first contain the x,y,z of new location.
		//Applicable: GeoObject
	EVENT_SET_PROGRAM,//A command to change the currently used program.
		//Arg Type: int, an index.
		//Args contain: first of first is an int index to the program of concern.
		//Applicable: Scene.
	EVENT_SET_CAMERA,//A command to change the current used camera.
		//Arg Type: int, an index.
		//Args contain: first of first is an int index to the desired camera.
		//Applicable: Scene.
	EVENT_ACTIVATE_SCENE,//A command to activate a scene into the render cycle.
		//Arg Type: UID
		//Args contain: a Scene to activate
		//Applicable: a yet unexistant class. Interface maybe?
	EVENT_DEACTIVATE_SECEN,//a command to take a scene out of the render cycle.
		//Arg Type:: UID
		//Args contain: A scene Id to deactivate
		//Applicable: Interface?
	EVENT_HALT,//This Event brings the entire system down, it gets broadcasted.
		//Arg Type:: Bool
		//Args contain: true.
		//Applicable: Resource Manager and Interface.
	EVENT_COUNT//This is a counter for all the number of Unique Events.
};

typedef int UID;
enum variant_t
{
	TYPE_BOOL,
	TYPE_CHAR,
	TYPE_SHORT,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_LONG,
	TYPE_DOUBLE,
	TYPE_COUNT//a trick I got from Jason Gregory. this enum counts the number 
			//of unique types. clever no?
};

typedef struct variant
{
	variant(char vals[16])
	{
		for(int i = 16; i-->0;datum.v_asChar[i] = vals[i]);
	}
	variant(variant& val)
	{
		datum.v_asLong[0] = val.datum.v_asLong[0];
		datum.v_asLong[1] = val.datum.v_asLong[1];
	}
	variant()
	{
		datum.v_asLong[0] = 0l;
		datum.v_asLong[1] = 0l;
	}
	variant_t type;
	union
	{
		bool v_asBool[ARG_SIZE/sizeof(bool)];
		char v_asChar[ARG_SIZE/sizeof(char)];
		short v_asShort[ARG_SIZE/sizeof(short)];
		int v_asInt[ARG_SIZE/sizeof(int)];
		float v_asFloat[ARG_SIZE/sizeof(float)];
		long int v_asLong[ARG_SIZE/sizeof(long int)];
		double v_asDouble[ARG_SIZE/sizeof(double)];
	} datum;
}Variant;

class Event
{
public:
	Event(UID _sender, event_t _type);
	virtual ~Event();
	bool setArg(Variant arg);
	bool setArgs(Variant arg1, Variant arg2, Variant arg3, Variant arg4);
	
	bool inline isBool(int arg){return args[arg].type==TYPE_BOOL;}
	bool inline isChar(int arg){return args[arg].type==TYPE_CHAR;}
	bool inline isShort(int arg){return args[arg].type==TYPE_SHORT;}
	bool inline isInt(int arg){return args[arg].type==TYPE_INT;}
	bool inline isFloat(int arg){return args[arg].type==TYPE_FLOAT;}
	bool inline isLong(int arg){return args[arg].type==TYPE_LONG;}
	bool inline isDouble(int arg){return args[arg].type==TYPE_DOUBLE;}
private:
	event_t type;
	Variant args[ARG_COUNT];
	UID sender;
};


/*.S.D.G.*/
#endif
