/* Program:	Delta's interface for openGL
 * Programer:	Toben "Littlefoot" Archer
 * Date:	12/01/17
 * Purpose:	Abstract the specifics of openGL away from Delta, in the future
	This will server also for dealing with any differences between the OSs
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

class Interface
{
public:
	Interface();
	~Interface();
	void init(const char* title, int _x, int _y);
	int go();

	void keyFunc(unsigned char key, int x, int y);
	void motionFunc(int x, int y);
	void passiveMotionFunc(int x, int y);
	void mouseFunc(int button, int state, int x, int y);
//	void displayFunc();
//	void idleFunc();
	void reshapeFunc(int width, int height);

	void setKeyFunc(void(*func)(unsigned char key, int x, int y));
	void setMotionFunc(void(*func)(int x, int y));
	void setPassiveMotionFunc(void(*func)(int x, int y));
	void setMouseFunc(void(*func)(int button, int state, int x, int y));
	void setDisplayFunc(void(*func)());
	void setIdleFunc(void(*func)());
	void setReshapeFunc(void(*func)(int width, int height));

//	void inline setProg(Program* val){ prog = val; }
//	void inline setCam(Camera* val){ cam = val; }
//	void inline setCoord(Attrib* val){ coord = val; }
//	void inline setColor(Attrib* val){ color = val; }

private:
	//data members:
	int mx, my;		//container for mouse location
	int lbutton, rbutton;	//container for mouse button pressings.
	void (*keyFuncs[256])(void);

};
static Interface* globalIn;
/*.S.D.G.*/
#endif
