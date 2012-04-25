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


#endif

/*

//public:
InterfaceGL::InterfaceGL()
{
	x = 800;
	y = 600;
	mx = my = 0;
	lbutton = rbutton = 0;
	for(int i = 256; i-->0; keyFuncs[i] = NULL);
}
InterfaceGL::~InterfaceGL()
{
	delete keyFuncs;
	prog = NULL;
	cam = NULL;
	coord = NULL;
	color = NULL;
	x = y = mx = my = lbutton = rbutton = 0;
}
void InterfaceGL::init(const char* title, int _x, int _y)
{
//	char** temp = [title,""];
//	glutInit(0,temp);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(_x, _y);
	glutCreateWindow(title);
	x = _x; y = _y;

//	glEnable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_POINT_SMOOTH);
//	glEnable(GL_PROGRAM_POINT_SIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	}

	if (!GLEW_VERSION_2_0) 
	{
		fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
	}
}

int InterfaceGL::go()
{
	if (prog == NULL) return 1;
	#if _WIN64 || __amd64__
	printf("prog: %li\n",(long)prog);
	#else
	printf("prog: %i\n",(int)prog);
	#endif
	if (prog->getProgram() == 0) return 2;
	if (cam == NULL) return 3;
	#if _WIN64 || __amd64__
	printf("cam: %li\n",(long)cam);
	#else
	printf("cam: %i\n",(int)cam);
	#endif
	if (coord == NULL) return 4;
	if (color == NULL) return 5;
	glutMainLoop();
	return 0;
}

//private:
void InterfaceGL::keyFunc(unsigned char key, int x, int y)
{
	mx = x;
	my = y;
	if (keyFuncs[key] != NULL) keyFuncs[key]();
}
void InterfaceGL::motionFunc(int x, int y)
{
	mx = x;
	my = y;
}
void InterfaceGL::passiveMotionFunc(int x, int y)
{
	mx = x;
	my = y;
}
void InterfaceGL::mouseFunc(int button, int state, int x, int y)
{
	mx = x;
	my = y;
	if (button == GLUT_LEFT_BUTTON)
		lbutton = state;
	else if (button == GLUT_RIGHT_BUTTON)
		rbutton = state;
}
void InterfaceGL::displayFunc()
{
	glutSwapBuffers();
}
void InterfaceGL::idleFunc()
{
	glutPostRedisplay();
}
void InterfaceGL::reshapeFunc(int width, int height)
{
	x = width;
	y = height;
	glViewport(0, 0, x, y);
}
void InterfaceGL::setKeyFunc(void(*func)(unsigned char key, int x, int y))
{
	glutKeyboardFunc(func);
}
void InterfaceGL::setMotionFunc(void(*func)(int x, int y))
{
	glutMotionFunc(func);
}
void InterfaceGL::setPassiveMotionFunc(void(*func)(int x, int y))
{
	glutPassiveMotionFunc(func);
}
void InterfaceGL::setMouseFunc(void(*func)(int button, int state, int x, int y))
{
	glutMouseFunc(func);
}
void InterfaceGL::setDisplayFunc(void(*func)())
{
	glutDisplayFunc(func);
}
void InterfaceGL::setIdleFunc(void(*func)())
{
	glutIdleFunc(func);
}
void InterfaceGL::setReshapeFunc(void(*func)(int width, int height))
{
	glutReshapeFunc(func);
}

/*.S.D.G.*/
