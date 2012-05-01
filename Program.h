/* This class holds everything nescessary to deal with a GLSL program. which is:
 * Optional Geometry Shader.
 * Vertex Shader.
 * Fragment Shader.
 * It will hold this info as well as deal with loading, compiling, and linking.
 */
#ifndef PROGRAM_H
#define PROGRAM_H
#include <stdio.h>
#include <stdlib.h>
#include "Object.cpp"

#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;
class Program: public Object
{
public:
	Program();
	~Program();
	Program(xml_node self, string path);
	
	//another trick like i used in the camera class. I'd hope that eventually I could remove 
	//this function, but for now it remains to fill in the gaps where necessary.
	GLuint inline getProgram() { return newProg ? prog : updateProg(); }
	int loadVertex(const char* fileName);
	int loadFragment(const char* fileName);
	void inline use();
	GLint inline getVP(){return VP;}
	GLint inline getLocal(){return local;}
	void trip(xml_node arg);
private:
	//data members:
	//int usingGeom;
	//GLuint gs, vs, fs;
	GLuint vs, fs;	//pointers to the shanders on the gpu.
	GLuint prog;	//pointer to the program on the gpu.
	int newProg;	//sentinal value for if the program needs to be updated or not.
	GLint VP;
	GLint local;

	//member functions:
	GLuint updateProg();
	char* file_read(const char* filename);
	GLint create_shader(const char* filename, GLenum type);
	void print_log(GLuint object);
};
/*.S.D.G.*/
#endif
