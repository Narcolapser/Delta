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
	ID = (UID)1;
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

}

void Interface::update()
{
	TRACE(1);
	for(int i = 0; i<256; ++i)
	{
		TRACE(2);
		if(normalKeys[i])
		{
			TRACE(3);
			for(int j = 0; j < normalEvents[i].size(); ++j)
			{
				TRACE(5);
				normalEvents[i][j].recv->onEvent(normalEvents[i][j].send);
			}
		}
	}
}
void Interface::registerToExternal(int key, Object* val, const Event& event)
{
	normalEvents[key].push_back(recEvent(event,val));
}
void keyFunc(unsigned char key, int x, int y)
{
	globalIn->normalKeys[key]=true;
	printf("Raised: %c #%i\n",key,(int)key);
}
void specialFunc(int key, int x, int y)
{
	printf("key: %i\n",key);
}
void keyUpFunc(unsigned char key, int x, int y)
{
	globalIn->normalKeys[key]=false;
	printf("Lowered: %c #%i\n",key,(int)key);
}
void specialUpFunc(int key, int x, int y)
{

}
bool Interface::onEvent(const Event& event)
{

//	Event move2((UID)0,EVENT_MOVE);
//	move2.setArgs(0.0f,0.0f,-0.01f,0.0f);
//	globalIn->registerToExternal(103,((Object*)foo->models[1]),move2);	

	Event* e;

	//the cases to this switch are arbitrarily assigned values.
	//1 = move object
	//2 = rotate object
	//3 = spawn object
	switch(event.args[0].datum.v_asInt[0])
	{
		case 1:
			e = new Event();
			e->sender = ID;
			e->receiver = globalRM->ResolveLTI(event.args[2].datum.v_asChar);
			e->args[0] = event.args[1];
			e->type = EVENT_MOVE;
			registerToExternal(event.args[0].datum.v_asInt[1],globalRM->GetIDNonRetaining(e->receiver),*e);
			break;
		case 2:
			e = new Event();
			e->sender = ID;
			e->receiver = globalRM->ResolveLTI(event.args[2].datum.v_asChar);
			e->args[0] = event.args[1];
			e->type = EVENT_ROTATE;
			registerToExternal(event.args[0].datum.v_asInt[1],globalRM->GetIDNonRetaining(e->receiver),*e);
			break;

		case 3:
			e = new Event();
			e->sender = ID;
			e->receiver = globalRM->ResolveLTI(event.args[2].datum.v_asChar);
			e->args[0].datum.v_asInt[0] = event.args[0].datum.v_asInt[2];
			e->args[0].datum.v_asFloat[1] = event.args[1].datum.v_asFloat[0];
			e->args[0].datum.v_asFloat[2] = event.args[1].datum.v_asFloat[1];
			e->args[0].datum.v_asFloat[3] = event.args[1].datum.v_asFloat[2];
			e->type = EVENT_SPAWN_OBJECT;
			registerToExternal(event.args[0].datum.v_asInt[1],globalRM->GetIDNonRetaining(e->receiver),*e);
			break;
		default:
			break;
	}
	return true;
}

/*.S.D.G.*/

#endif
