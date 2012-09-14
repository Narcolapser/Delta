/* Interface is just a wrapper class, it actually does not deal at all with the 
 * actual creation of the lower peices, it simply holds them. It is the Interface
 * that has the responsibility to bring the peices together, you interface to the
 * other sub peices out side of this, that's what makes this system convenient.
 * you will notice however that the interface does keep the size of the display.
 * While cameras do this to aswell, the cameras may not always be concerned with
 * what the parent window is actually at. for example a camera that is for a 
 * HUD overlay doesn't care what the main window size is. This keeps track of the
 * main window size so that when a camera is punched in, it can choose to take
 * that info or not.
 * Also, mx and my are the most recent position of the mouse. and lbutton and
 * rbutton hold the most recent state of the left and right mouse buttons.
 */
#ifndef INTERFACE_CPP
#define INTERFACE_CPP
#include "Interface.h"

Interface::Interface()
{
	//the interface is given special assignment for UID of 1. 
	ID = (UID)1;
	//this initalizes all the special key assignments. 
	specialKeys[0] = false;
	specialKeys[1] = false;
	specialKeys[2] = false;
	specialKeys[3] = false;
	specialKeys[4] = false;
	specialKeys[5] = false;
	specialKeys[6] = false;
	specialKeys[7] = false;
	specialKeys[8] = false;
	specialKeys[9] = false;
	specialKeys[10] = false;
	specialKeys[11] = false;
	specialKeys[12] = false;
	specialKeys[100] = false;
	specialKeys[101] = false;
	specialKeys[102] = false;
	specialKeys[103] = false;
	specialKeys[104] = false;
	specialKeys[105] = false;
	specialKeys[106] = false;
	specialKeys[107] = false;
	specialKeys[108] = false;
//	specialEvents[0] = vector<recEvent>;
	printf("Interface instanciated!\n");
}

Interface::~Interface()
{
	//there really isn't anything that needs to be deconstructed.
}

void Interface::update()
{//this loop goes through all of the normal key events and dispatches them accordingly.
	TRACE(1);
	for(int i = 0; i<256; ++i)
	{
		TRACE(2);
		if(normalKeys[i])
		{//check if this key is down.
			TRACE(3);
			for(int j = 0; j < normalEvents[i].size(); ++j)
			{//this loop does the actual dispatching.
				TRACE(5);
				Object* temp = normalEvents[i][j].recv;TRACE(5);
				Event stemp;
				stemp = normalEvents[i][j].send;TRACE(5);
				temp->onEvent(stemp);TRACE(5);
			}
		}
	}
}
void Interface::registerToExternal(int key, Object* val, const Event& event)
{//this registers an event to a key being pressed.
	normalEvents[key].push_back(recEvent(event,val));
}
void keyFunc(unsigned char key, int x, int y)
{//function to pass to GLUT for managing a key being pressed.
	globalIn->normalKeys[key]=true;
	printf("Raised: %c #%i\n",key,(int)key);
}
void specialFunc(int key, int x, int y)
{//function to pass to GLUT for managing special keys being pressed.
	printf("key: %i\n",key);
}
void keyUpFunc(unsigned char key, int x, int y)
{//function to pass to GLUT for keys being raised.
	globalIn->normalKeys[key]=false;
	printf("Lowered: %c #%i\n",key,(int)key);
}
void specialUpFunc(int key, int x, int y)
{
	//never got around writting this function, but it would manage special keys being released.
}
bool Interface::onEvent(const Event& event)
{
	TRACE(1);
	Event* e;

	//the cases to this switch are arbitrarily assigned values.
	//1 = move object
	//2 = rotate object
	//3 = spawn object
	switch(event.args[0].datum.v_asInt[0])
	{
		case 1:
			TRACE(3);
			e = new Event();TRACE(5);
			e->sender = ID;TRACE(5);
			e->receiver = globalRM->ResolveLTI(event.args[2].datum.v_asChar);TRACE(5);
			e->args[0] = event.args[1];TRACE(5);
			e->type = EVENT_MOVE;TRACE(5);
			registerToExternal(event.args[0].datum.v_asInt[1],globalRM->GetIDNonRetaining(e->receiver),*e);TRACE(5);
			break;
		case 2:
			TRACE(3);
			e = new Event();TRACE(5);
			e->sender = ID;TRACE(5);
			e->receiver = globalRM->ResolveLTI(event.args[2].datum.v_asChar);TRACE(5);
			e->args[0] = event.args[1];TRACE(5);
			e->type = EVENT_ROTATE;TRACE(5);
			registerToExternal(event.args[0].datum.v_asInt[1],globalRM->GetIDNonRetaining(e->receiver),*e);TRACE(5);
			break;

		case 3:
			TRACE(3);
			e = new Event();TRACE(5);
			e->sender = ID;TRACE(5);
			e->receiver = globalRM->ResolveLTI(event.args[2].datum.v_asChar);TRACE(5);
			e->args[0].datum.v_asInt[0] = event.args[0].datum.v_asInt[2];TRACE(5);
			e->args[0].datum.v_asFloat[1] = event.args[1].datum.v_asFloat[0];TRACE(5);
			e->args[0].datum.v_asFloat[2] = event.args[1].datum.v_asFloat[1];TRACE(5);
			e->args[0].datum.v_asFloat[3] = event.args[1].datum.v_asFloat[2];TRACE(5);
			e->type = EVENT_SPAWN_OBJECT;TRACE(5);
			registerToExternal(event.args[0].datum.v_asInt[1],globalRM->GetIDNonRetaining(e->receiver),*e);TRACE(5);
			break;
		default:
			TRACE(3);
			break;
	}
	return true;
}

/*.S.D.G.*/

#endif
