/**
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 * 	Toben "Littlefoot" Archer
 */

#include <math.h>
#include <iostream>
#include "Mesh.cpp"
#include "InterfaceGL.cpp"
#include "Camera.h"
#include "Program.h"
#include "Attrib.h"

using namespace std;

Camera *c;
Program *prog;
Attrib *coord3d,*vColor,*elArr;
Attrib *coord3df,*vColorf,*elArrf;
Mesh *cube;
//GLuint vbo_mesh_vertices, vbo_mesh_normals, ibo_mesh_elements;
GLuint vbo_cube_vertices, vbo_cube_colors, ibo_cube_elements;
GLuint vbo_floor_vertices, vbo_floor_colors, ibo_floor_elements;

glm::mat4 model,view,projection,anim;

GLint uniform_mvp;
GLint local;
bool rotate_l = false;
bool rotate_r = false;
float angle = 0.0;
float angleV = 0.0;
float angleH = 0.0;
float zoom = -4.0;
int lx = -1, ly = -1;
bool lclick = false;
bool rclick = false;
float shuffle = 0.0;

int init_resources()
{
	c = new Camera(45.0f,800,600,0.1f,1000.0f,glm::vec3(0.0, 2.0, 0.0));

	//ATTRIBUTE DATA:////////////////////////////////////////////////////////////////////
	GLfloat floor_vertices[] = {
		-2.0, -2.0, -2.0,
		 2.0, -2.0, -2.0,
		 2.0, -2.0,  2.0,
		-2.0, -2.0,  2.0,
	};
	glGenBuffers(1, &vbo_floor_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

	GLushort floor_elements[] = {
		//floor
		0, 1, 2,
		0, 2,3,
	};
	glGenBuffers(1, &ibo_floor_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_floor_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_elements), floor_elements, GL_STATIC_DRAW);

	//PROGRAM:////////////////////////////////////////////////////////////////////
	prog = new Program();
	if (prog->loadVertex("Shaders/cube.v.glsl") == 0) return 0;
	if (prog->loadFragment("Shaders/cube.f.glsl") == 0) return 0;

	GLint link_ok = GL_FALSE;

	const char* filename = "sphere.obj";
	cube = new Mesh(prog,filename);

	//ATTRIBUTE SPEC:////////////////////////////////////////////////////////////////////
	const char* attribute_name;
	attribute_name = "coord3d";
	coord3df = new Attrib(prog,attribute_name,GL_FLOAT,vbo_floor_vertices,GL_ARRAY_BUFFER);
	attribute_name = "elArr";
	elArrf = new Attrib(prog,attribute_name,GL_ELEMENT_ARRAY_BUFFER,ibo_floor_elements,GL_ELEMENT_ARRAY_BUFFER);

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

	return 1;
}

void onIdle() {
	glm::vec3 axis_x(1, 0, 0);
	anim = glm::rotate(glm::mat4(1.0f), angleV, axis_x);

	glm::vec3 axis_y(0, 1, 0);
	anim = glm::rotate(anim, angleH, axis_y);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, zoom));
	view = c->lookAt(glm::vec3(0.0, 0.0, zoom));
	projection = c->getProjection();

	glm::mat4 mvp = projection * view * model;

	prog->use();
	glUniformMatrix4fv(local, 1, GL_FALSE, glm::value_ptr(anim));
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(local,0.0);
	glutPostRedisplay();
}

void onDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	prog->use();
	int size;	

	coord3df->enable();
	coord3df->bind();
	coord3df->pointer();
	elArrf->bind();
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	coord3df->disable();

	cube->coords->enable();
	cube->coords->bind();
	cube->coords->pointer();
	cube->elements->bind();
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	cube->coords->disable();

//	cube->setTrans(glm::translate(glm::mat4(1.0f), glm::vec3(0.0,shuffle,0.0)));
//	cube->setTrans(anim);
//	cube->render(local);

	glutSwapBuffers();
}

void onReshape(int width, int height) {
	c->setScreen(width,height);
	glViewport(0, 0, width, height);
}

void onNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
//	cout << "key: " << (char) key << " Value of: " << (int) key << " x: " << x << " y: " << y << endl;
	if (key == 'a') shuffle++;
	if (key == 'd') shuffle--;
}

void onActiveMotion(int x, int y)
{
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
	else if(rclick)
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
{
	lx = x;
	ly = y;
}

void onClick(int button, int state, int x, int y)
{
	lx = x;
	ly = y;
	if (button == GLUT_LEFT_BUTTON)
		lclick = state == GLUT_DOWN;
	else if (button == GLUT_RIGHT_BUTTON)
		rclick = state == GLUT_DOWN;
}
	

void free_resources()
{
	delete prog;
	glDeleteBuffers(1, &vbo_cube_vertices);
	glDeleteBuffers(1, &vbo_cube_colors);
	glDeleteBuffers(1, &ibo_cube_elements);
}


int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Delta Alpha 5");
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
