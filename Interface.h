/* Program:	Delta's interface for openGL
 * Programer:	Toben "Littlefoot" Archer
 * Date:	12/01/17
 * Purpose:	Abstract the specifics of openGL away from Delta, in the future
	This will also deal with any differences between the OSs
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>

#include "Event.cpp"
#include "Object.cpp"
#include "ResourceManager.h"

using namespace std;

class Interface: public Object
{
public:
	Interface();
	virtual ~Interface();
	void update();
	void registerToExternal(int key, Object* val, const Event& event);

	void motionFunc(int x, int y);
	void passiveMotionFunc(int x, int y);
	void mouseFunc(int button, int state, int x, int y);
	virtual bool onEvent(const Event& event);

//private:
	//data members:
	int mx, my;		//container for mouse location
	int lbutton, rbutton;	//container for mouse button pressings.
	bool normalKeys[256];
	map <int,bool> specialKeys;
	struct recEvent
	{
		recEvent();
		recEvent(const recEvent& val)
		{
			recv = val.recv;
			send = val.send;
		}
		recEvent(const Event& event, Object* val)
		{
			recv = val;
			send = event;
		}
		Object* recv;
		Event send;
	};
	vector<recEvent> normalEvents[256];
	map< int, vector<recEvent> > specialEvents;

};
void keyFunc(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
void keyUpFunc(unsigned char key, int x, int y);
void specialUpFunc(int key, int x, int y);

static Interface* globalIn;
/*.S.D.G.*/
#endif
