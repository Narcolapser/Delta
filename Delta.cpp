/**
 * This file is in the public domain.
 * Contributors: Toben "Littlefoot" Archer
 * 	Sylvain Beucler
 */

#include <math.h>
#include <iostream>
#include "Mesh.cpp"
//#include "InterfaceGL.cpp"
#include "Camera.h"
#include "Program.h"
#include "Attrib.h"
#include "Buffer.h"
#include "Scene.cpp"

using namespace std;

//hopefully these global variables will be going away soon.
Camera *c;
Program *prog;
Attrib *coord3d,*vColor,*elArr;
Attrib *coord3df,*vColorf,*elArrf;
Mesh *cube;
Mesh *child;
Mesh *plate;
Scene *foo;
ResourceManager *bar;
glm::mat4 model,view,projection,anim;

GLint uniform_mvp;
GLint local;
float angle = 0.0;
float angleV = 0.0;
float angleH = 0.0;
float rotx = 0.0;
float roty = 0.0;
float rotz = 0.0;
float zoom = -10.0;
int lx = -1, ly = -1;
bool lclick = false;
bool rclick = false;
float shuffle = 0.0;

//initalize all the resources for Delta.
int init_resources()
{
	//seed the random number generator.
	srand(time(NULL));

	//The camera. this is new since my refactoring. 
	c = new Camera(45.0f,800,600,0.1f,1000.0f,glm::vec3(0.0, 2.0, 0.0));

	//PROGRAM:////////////////////////////////////////////////////////////////////
	//this handles the creation of the program. loading the shaders and linking.
	prog = new Program();
	if (prog->loadVertex("Shaders/cube.v.glsl") == 0) return 0;
	if (prog->loadFragment("Shaders/cube.f.glsl") == 0) return 0;


	//MESHES! this is where the meshes get loaded.
//	const char* filename = "SceneResources/cubequad.obj";
//	cube = new Mesh(filename);
//	cube->bindToProg(prog);
//	printf("first mesh ID: %i\n",cube->getID());
//	c->setParent(cube);
//	c->setFocus(cube);
//	child = new Mesh(filename);
//	child->bindToProg(prog);
//	printf("child ID: %i\n",cube->getID());
//	child->move(-1,3,-1);
//	child->setParent(cube);
//	filename = "SceneResources/floor.obj";
//	plate = new Mesh(filename);
//	plate->bindToProg(prog);
//	printf("second mesh ID: %i\n",plate->getID());
//	filename = "Scene";
//	foo = new Scene(filename);
	const char* filename = "Scene.xml";
	bar = new ResourceManager();

	//There isn't a good way to deal with uniforms yet. I'm going to be doing something with
	//	them because uniform buffer objects will be imlemented soon. so these will remain
	//	for the time being.
	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(prog->getProgram(), uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "local";
	local = glGetUniformLocation(prog->getProgram(), uniform_name);
	if (local == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

//	foo->bindToProgram(prog,local);
	//printf("Resources loaded!\n");
	return 1;//resources initalized.
}

void onIdle() 
{
	//the on idle function. basically this gets called when the frame has been rendered. 
	//	in the future this might just raise an event because I'd like to push the rendering
	//	controller to a seperate thread, but for now it handles the animation process.

	//these two rotates deal with rotating the camera on the vertical axis and horizontal axis.
	//	I'm going to have to figure out how to reimplement them later.
	glm::vec3 axis_x(1, 0, 0);
	anim = glm::rotate(glm::mat4(1.0f), angleV, axis_x);

	glm::vec3 axis_y(0, 1, 0);
	anim = glm::rotate(anim, angleH, axis_y);

	//make the global move. right now it just manages the zooming in and out of the camera.
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, zoom));

	//get the view and projection matrix from the camera.
	view = c->lookAt(glm::vec3(0.0, 0.0, zoom));
//	view = c->view();
	projection = c->getProjection();

	//make the model-view-projection global transformation matrix.
	glm::mat4 mvp = projection * view * model * anim;

	//declare that we are going to use this program. woohoo and all that.
	prog->use();

	glUniformMatrix4fv(local, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		//set the uniform "local" to be this anim transform because mesh specific 
		//	transforms are not currently working.

	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
		//set the global trasnform

	glutPostRedisplay();//request a redraw.
}

void onDisplay()
{//redraw!
	glClearColor(0.0, 0.0, 0.0, 1.0);//set the color to clear too. not the color that is clear.
		//thats what the alpha channel is for.
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear these buffers.
	prog->use();//use this program. woohoo and all that.
//	foo->render();

//	plate->setTrans(anim);
//	plate->rotate(rotx,roty,rotz);
//	plate->render(local);//render the plate.

//	glm::vec3 axis_x(1, 0, 0);
//	anim = glm::rotate(glm::mat4(1.0f), angleV, axis_x);

//	glm::vec3 axis_y(0, 1, 0);
//	anim = glm::rotate(anim, angleH, axis_y);
//	cube->setTrans(glm::translate(glm::mat4(1.0f), glm::vec3(-shuffle,0.0,0.0)));

//	cube->setLoc(glm::vec3(-shuffle,0.0,0.0));
//	cube->move(-shuffle,0,0);
//	foo->meshes[1]->move(-shuffle,0,0);
//	cube->render(local);//render the cube, which is currently a sphere...
//	child->render(local);

	shuffle *= 0.9;
	rotx *= 0.9;
	roty *= 0.9;
	rotz *= 0.9;
//	cout << "x: " << rotx << " y: " << roty << " z: " << rotz << endl;
//	cout << "Shuffle: " << shuffle << endl;
	//redrawn, swap the buffers and put this new one to the front.
	glutSwapBuffers();
}

void onReshape(int width, int height) 
{//what to do when the user changes the size of the screen.
	c->setScreen(width,height);
	glViewport(0, 0, width, height);
}

void onNormalKeys(unsigned char key, int x, int y)
{//what to do on key strokes.
	if (key == 27) exit(0);
//	cout << "key: " << (char) key << " Value of: " << (int) key << " x: " << x << " y: " << y << endl;
	if (key == 'a') shuffle++;
	if (key == 'd') shuffle--;
	if (key == 'x') rotx += 1;
	if (key == 'y') roty += 1;
	if (key == 'z') rotz += 1;
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
		if (dy > 0)
		{
			dy *= (-1);
			if (dy <= 1) dy = 2;
			zoom += log10(dy);
		}
		else
		{
			if (dy <= 1) dy = 2;
			zoom += log10(dy) * (-1);
		}
		if (zoom > -1.5) zoom = -1.5;
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
	

void free_resources()
{//done, free up the resources so there aren't any loose ends.
	delete prog;
	delete cube;
	delete plate;
}


int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Delta Alpha 8");
	glutKeyboardFunc(onNormalKeys);
	glutMotionFunc(onActiveMotion);
	glutPassiveMotionFunc(onPassiveMotion);
	glutMouseFunc(onClick);

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

	free_resources();
	return 0;
}

/*.S.D.G.*/
