/* Program:	Delta's interface for openGL
 * Programer:	Toben "Littlefoot" Archer
 * Date:	12/01/17
 * Purpose:	Abstract the specifics of openGL away from Delta, in the future
	This will server also for dealing with any differences between the OSs
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Attrib.h"
#include "Camera.h"
#include <vector>

using namespace std;

class InterfaceGL
{
public:
	InterfaceGL();
	~InterfaceGL();
	void init(const char* title, int _x, int _y);
	int go();

	void keyFunc(unsigned char key, int x, int y);
	void motionFunc(int x, int y);
	void passiveMotionFunc(int x, int y);
	void mouseFunc(int button, int state, int x, int y);
	void displayFunc();
	void idleFunc();
	void reshapeFunc(int width, int height);

	void setKeyFunc(void(*func)(unsigned char key, int x, int y));
	void setMotionFunc(void(*func)(int x, int y));
	void setPassiveMotionFunc(void(*func)(int x, int y));
	void setMouseFunc(void(*func)(int button, int state, int x, int y));
	void setDisplayFunc(void(*func)());
	void setIdleFunc(void(*func)());
	void setReshapeFunc(void(*func)(int width, int height));

	void inline setProg(Program* val){ prog = val; }
	void inline setCam(Camera* val){ cam = val; }
	void inline setCoord(Attrib* val){ coord = val; }
	void inline setColor(Attrib* val){ color = val; }

private:
	//data members:
	Program* prog;
	Camera* cam;
	Attrib* coord;
	Attrib* color;
	vector<float>* indecies;
	int x,y;
	int mx, my;
	int lbutton, rbutton;
	void (*keyFuncs[256])(void);

};
/*.S.D.G.*/
