/**
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */

#include <math.h>
#include <iostream>
//#include "Program.h"
#include "Camera.h"
#include "Attrib.h"

//#include <stdio.h>
//#include <stdlib.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
//#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
//#include <GL/glut.h>
/* GLM */
// #define GLM_MESSAGES
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

using namespace std;

Camera *c;
Program *prog;
Attrib *coord3d,*vColor,*elArr;
int screen_width=800, screen_height=600;
GLuint vbo_cube_vertices, vbo_cube_colors;
GLuint ibo_cube_elements;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_mvp;
bool rotate_l = false;
bool rotate_r = false;
float angle = 0.0;
float angleV = 0.0;
float angleH = 0.0;
float zoom = -4.0;
int lx = -1, ly = -1;
bool lclick = false;
bool rclick = false;

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
char* file_read(const char* filename)
{
	FILE* in = fopen(filename, "rb");
	if (in == NULL) return NULL;

	int res_size = BUFSIZ;
	char* res = (char*)malloc(res_size);
	int nb_read_total = 0;

	while (!feof(in) && !ferror(in)) {
		if (nb_read_total + BUFSIZ > res_size) {
			if (res_size > 10*1024*1024) break;
			res_size = res_size * 2;
			res = (char*)realloc(res, res_size);
		}
		char* p_res = res + nb_read_total;
		nb_read_total += fread(p_res, 1, BUFSIZ, in);
	}
	
	fclose(in);
	res = (char*)realloc(res, nb_read_total + 1);
	res[nb_read_total] = '\0';
	return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLint create_shader(const char* filename, GLenum type)
{
	const GLchar* source = file_read(filename);
	if (source == NULL) {
		fprintf(stderr, "Error opening %s: ", filename); perror("");
		return 0;
	}
	GLuint res = glCreateShader(type);
	glShaderSource(res, 1, &source, NULL);
	free((void*)source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "%s:", filename);
		print_log(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}

int init_resources()
{
	c = new Camera(45.0f,screen_width, screen_height,0.1f,10.0f,glm::vec3(0.0, 2.0, 0.0));

	//ATTRIBUTE DATA:////////////////////////////////////////////////////////////////////
	GLfloat cube_vertices[] = {
		// front
		-1.0, -1.0, 1.0,
		 1.0, -1.0, 1.0,
		 1.0,  1.0, 1.0,
		-1.0,  1.0, 1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
		-2.0, -2.0, -2.0,
		 2.0, -2.0, -2.0,
		 2.0, -2.0,  2.0,
		-2.0, -2.0,  2.0,
	};
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	GLfloat cube_colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		// back colors
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 0.0, 0.0,
		// floor colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		
	};
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

	GLushort cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
		//floor
		8, 9, 10,
		8, 10,11,
	};
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);


	//PROGRAM:////////////////////////////////////////////////////////////////////
	prog = new Program();
	if (prog->loadVertex("Shaders/cube.v.glsl") == 0) return 0;
	if (prog->loadFragment("Shaders/cube.f.glsl") == 0) return 0;
	program = prog->getProgram();

	GLint link_ok = GL_FALSE;

//	GLuint vs, fs, gs;
//	if ((vs = create_shader("Shaders/cube.v.glsl", GL_VERTEX_SHADER))	== 0) return 0;
//	if ((fs = create_shader("Shaders/cube.f.glsl", GL_FRAGMENT_SHADER))	== 0) return 0;
//	if ((gs = create_shader("Shaders/cube.f.glsl", GL_GEOMETRY_SHADER_EXT)) == 0) return 0;
//	program = glCreateProgram();
//	glAttachShader(program, vs);
//	glAttachShader(program, fs);

//	glAttachShader(program, gs);
//	glProgramParameteriEXT(program,GL_GEOMETRY_INPUT_TYPE_EXT,GL_POINTS);
//	glProgramParameteriEXT(program,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_TRIANGLES_ADJACENCY_EXT);

//	int temp;
//	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
//	glProgramParameteriEXT(program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);



//	glLinkProgram(program);
//	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
//	if (!link_ok) {
//		fprintf(stderr, "glLinkProgram:");
//		print_log(program);
//		return 0;
//	}

	//ATTRIBUTE SPEC:////////////////////////////////////////////////////////////////////
	//Attrib *coord3d,*vColor,*elArr;

	const char* attribute_name;
	attribute_name = "coord3d";
	coord3d = new Attrib(prog,attribute_name,GL_FLOAT,vbo_cube_vertices);
	attribute_name = "v_color";
	vColor = new Attrib(prog,attribute_name,GL_FLOAT,vbo_cube_colors);
//	attribute_name = "elArr";
//	elArr = new Attrib(prog,attribute_name,GL_ELEMENT_ARRAY_BUFFER,ibo_cube_elements);

//	attribute_coord3d = glGetAttribLocation(program, attribute_name);
//	if (attribute_coord3d == -1) {
//		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//		return 0;
//	}
//	attribute_name = "v_color";
//	attribute_v_color = glGetAttribLocation(program, attribute_name);
//	if (attribute_v_color == -1) {
//		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//		return 0;
//	}

	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	return 1;
}

void onIdle() {
//	if (rotate_l)
//	{
//		//angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;	// 45° per second
//		angle -= 2;
//		rotate_l = false;
//	}
//	else if (rotate_r)
//	{
//		angle += 2;
//		rotate_r = false;
//	}

	glm::vec3 axis_x(1, 0, 0);
	glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angleV, axis_x);	

	glm::vec3 axis_y(0, 1, 0);
	anim = glm::rotate(anim, angleH, axis_y);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, zoom));

//	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, zoom), glm::vec3(0.0, 1.0, 0.0));
//	glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 1000.0f);

	glm::mat4 view = c->lookAt(glm::vec3(0.0, 0.0, zoom));
	glm::mat4 projection = c->getProjection();

	glm::mat4 mvp = projection * view * model * anim;

	glUseProgram(program);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glutPostRedisplay();
}

void onDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
//	glEnableVertexAttribArray(attribute_coord3d);
////	 Describe our vertices array to OpenGL (it can't guess its format automatically)
//	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
//	glVertexAttribPointer(
//		attribute_coord3d,	 attribute
//		3,			 number of elements per vertex, here (x,y,z)
//		GL_FLOAT,		 the type of each element
//		GL_FALSE,		 take our values as-is
//		0,			 no extra data between each position
//		0			 offset of first element
//		);

//	glEnableVertexAttribArray(attribute_v_color);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
//	glVertexAttribPointer(
//		attribute_v_color,	// attribute
//		3,			// number of elements per vertex, here (R,G,B)
//		GL_FLOAT,		// the type of each element
//		GL_FALSE,		// take our values as-is
//		0,			// no extra data between each position
//		0			// offset of first element
//		);

	coord3d->enable();
	coord3d->bind();
	coord3d->pointer();
	vColor->enable();
	vColor->bind();
	vColor->pointer();

	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	coord3d->disable();
	vColor->disable();
//	glDisableVertexAttribArray(attribute_coord3d);
//	glDisableVertexAttribArray(attribute_v_color);
	glutSwapBuffers();
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

void onNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	cout << "key: " << (char) key << " Value of: " << (int) key << " x: " << x << " y: " << y << endl;
	if (key == 'a') rotate_l = true;
	if (key == 'd') rotate_r = true;
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
//		cout << "Delta x: " << dx << " Delta y: " << dy << endl;
//		cout << "angleH: " << angleH << " angleV: " << angleV << endl;
	}
	else if(rclick)
	{
		if (dy < 0)
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
//		cout << "Zoom: " << zoom << " dy: " << dy << " Log out: " << log10(dy) << endl;
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
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_cube_vertices);
	glDeleteBuffers(1, &vbo_cube_colors);
	glDeleteBuffers(1, &ibo_cube_elements);
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("My Rotating Cube");
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
//		glPointSize(8.0);

		//glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutMainLoop();
	}

	free_resources();
	return 0;
}
