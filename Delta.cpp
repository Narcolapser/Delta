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
	TRACE(1);
	globalRM = new ResourceManager();
	globalIn = new Interface();
	//seed the random number generator.
	srand(time(NULL));

	const char* filename = "Scene.xml";
	xml_document doc;
	xml_parse_result result;			TRACE(3);
	result = doc.load_file(filename);		TRACE(3);

	foo = new Scene(doc.first_child());		TRACE(3);
	foo->bindToProgram(0);				TRACE(3);

	globalRM->ResolveRequests();			TRACE(3);

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


	//redrawn, swap the buffers and put this new one to the front.
	glutSwapBuffers();
}

void free_resources()
{//done, free up the resources so there aren't any loose ends.
	delete foo;
}


void shutdown()
{
	free_resources();
	printf("frames: %i\n",frameCount);
	exit(0);
}

int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Delta Alpha 13");
	glutKeyboardFunc(keyFunc);
	glutKeyboardUpFunc(keyUpFunc);
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
