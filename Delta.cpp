/**
 * This file is in the public domain.
 * Programmer: Toben "Littlefoot" Archer
 */

#include <math.h>
#include <iostream>
#include "Camera.cpp"
#include "ResourceManager.cpp"
#include "Scene.cpp"
#include "Interface.cpp"

using namespace std;

//hopefully these global variables will be going away soon.
Camera *c;
Program *prog;
Scene *foo;
glm::mat4 projection,view;

GLint uniform_mvp;
GLint local;

float angle = 0.0;
float angleV = 0.0;
float angleH = 0.0;
float rotx = 0.0;
float roty = 0.0;
float rotz = 0.0;
float zoom = 0.0;
int lx = -1, ly = -1;
bool lclick = false;
bool rclick = false;
float shuffle = 0.0;
float downShuf = 0.0;
int frameCount = 0;

//initalize all the resources for Delta.
int init_resources()
{
	globalRM = new ResourceManager();
	globalIn = new Interface();
	//seed the random number generator.
	srand(time(NULL));

	//PROGRAM:////////////////////////////////////////////////////////////////////
	//this handles the creation of the program. loading the shaders and linking.
//	prog = new Program();
//	if (prog->loadVertex("Shaders/cube.v.glsl") == 0) return 0;
//	if (prog->loadFragment("Shaders/cube.f.glsl") == 0) return 0;

	const char* filename = "Scene.xml";
	xml_document doc;
	xml_parse_result result;
	result = doc.load_file(filename);

	foo = new Scene(doc.first_child());
	c = foo->cameras[0];
	c->setFocus(foo->models[1]);
	foo->bindToProgram(0);

	Event move((UID)0,EVENT_MOVE);
	move.setArgs(0.0f,0.0f,0.01f,0.0f);
	globalIn->registerToExternal(104,((Object*)foo->models[1]),move);

	Event move2((UID)0,EVENT_MOVE);
	move2.setArgs(0.0f,0.0f,-0.01f,0.0f);
	globalIn->registerToExternal(103,((Object*)foo->models[1]),move2);

	Event rotX((UID)0,EVENT_ROTATE);
	rotX.setArgs(0.01f,0.0f,0.0f,0.0f);
	globalIn->registerToExternal(120,((Object*)foo->models[1]),rotX);

	Event rotY((UID)0,EVENT_ROTATE);
	rotY.setArgs(0.0f,0.01f,0.0f,0.0f);
	globalIn->registerToExternal(121,((Object*)foo->models[1]),rotY);

	Event rotZ((UID)0,EVENT_ROTATE);
	rotZ.setArgs(0.0f,0.0f,0.01f,0.0f);
	globalIn->registerToExternal(122,((Object*)foo->models[1]),rotZ);


	globalRM->ResolveRequests();

	printf("Resources loaded!\n");
	return 1;//resources initalized.
}

void onIdle() 
{
	frameCount++;
	globalIn->update();
	glutPostRedisplay();//request a redraw.
}

void onDisplay()
{//redraw!
	glClearColor(0.0, 0.0, 0.0, 1.0);//set the color to clear too. not the color that is clear.
		//thats what the alpha channel is for.
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear these buffers.

	TRACE(1);
	foo->render();
	TRACE(1);

//	Event move((UID)0,EVENT_MOVE);
//	move.setArgs(0.0f,downShuf,shuffle,0.0f);
//	((Object*)foo->models[1])->onEvent(move);

//	Event rot((UID)0,EVENT_ROTATE);
//	rot.setArgs(rotx,roty,rotz,0.0f);
//	((Object*)foo->models[1])->onEvent(rot);

	Event moveCam((UID)0,EVENT_MOVE);
	moveCam.setArgs(0.0f,0.0f,zoom,0.0f);
	((Object*)c)->onEvent(moveCam);

//	foo->models[1]->rotate(rotx,roty,rotz);
//	c->move(0.0,0.0,zoom);
//	shuffle *= 0.9;rotx *= 0.9;roty *= 0.9;rotz *= 0.9;zoom *= 0.9;downShuf*=0.9;

	//redrawn, swap the buffers and put this new one to the front.
	glutSwapBuffers();
//	printf("end render\n");
}

void onReshape(int width, int height) 
{//what to do when the user changes the size of the screen.
	c->setScreen(width,height);
	glViewport(0, 0, width, height);
}

void free_resources()
{//done, free up the resources so there aren't any loose ends.
//	delete prog;
	delete foo;
}


void shutdown()
{
	free_resources();
	printf("frames: %i\n",frameCount);
	exit(0);
}

void onNormalKeys(unsigned char key, int x, int y)
{//what to do on key strokes.
	if (key == 27) shutdown();
//	cout << "key: " << (char) key << " Value of: " << (int) key << " x: " << x << " y: " << y << endl;
	if (key == 'a') shuffle++;
	if (key == 'd') shuffle--;
	if (key == 'w') downShuf++;
	if (key == 's') downShuf--;
	if (key == 'x') rotx += 1;
	if (key == 'y') roty += 1;
	if (key == 'z') rotz += 1;
	keyFunc(key,x,y);
}

void onActiveMotion(int x, int y)
{//what to do when the user is clicking and dragging.
	int dx = x - lx;
	int dy = y - ly;

	if(lclick)
	{
		//add the change in movement to the angle
		angleV += dy;
		if (angleV < 0)//this will keep angleV inside of 0-360
			angleV += 360;
		else if (angleV > 360)
			angleV -= 360;
	
		//if the object is upside down the user will expect movement along the horizontal axis
		//to flip, or inother words, that he's always grabbing the front of the object.
		if ((int)angleV % 360 < 90 || (int)angleV % 360 > 270)
			angleH += dx;
		else
			angleH -= dx;
		if (angleH < 0)
			angleH += 360;
		else if (angleH > 360)
			angleH -= 360;
	}
	if(rclick)
	{
		zoom += dy;
//		if (dy > 0)
//		{
//			dy *= (-1);
//			if (dy <= 1) dy = 2;
//			zoom += log10(dy);
//		}
//		else
//		{
//			if (dy <= 1) dy = 2;
//			zoom += log10(dy) * (-1);
//		}
//		if (zoom > -1.5) zoom = -1.5;
	}
		
	lx = x;
	ly = y;
}

void onPassiveMotion(int x, int y)
{//what tod o when the user is just running his mouse across the screen.
	lx = x;
	ly = y;
}

void onClick(int button, int state, int x, int y)
{//what to do when the user clicks.
	lx = x;
	ly = y;
	if (button == GLUT_LEFT_BUTTON)
		lclick = state == GLUT_DOWN;
	else if (button == GLUT_RIGHT_BUTTON)
		rclick = state == GLUT_DOWN;
}

int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Delta Alpha 13");
	glutKeyboardFunc(onNormalKeys);
	glutKeyboardUpFunc(keyUpFunc);
	glutMotionFunc(onActiveMotion);
	glutPassiveMotionFunc(onPassiveMotion);
	glutMouseFunc(onClick);
	glutSpecialFunc(specialFunc);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	if (argc > 1)
		DEBUG = ((int)argv[1][0])-(int)'0';
	else
		DEBUG = 0;

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}

	if (init_resources()) {
		glutDisplayFunc(onDisplay);
		glutReshapeFunc(onReshape);
		glutIdleFunc(onIdle);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutMainLoop();
	}
	return 0;
}

/*.S.D.G.*/
