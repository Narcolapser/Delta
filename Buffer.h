/* this holds data for interfacing with the graphics card, both from OpenGl and OpenCL. These data
 * are held in buffered objects. Buffer objects are also how OpenGL and OpenCL share information
 * and as such this class will also be responsible for creating the interopability between them on
 * these buffer objects. 
 * It contains:
 * GL/CL Buffer object reference
 * size of buffer
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "Object.cpp"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

using namespace std;

class Buffer: public Object
{
public:
	Buffer(GLenum _bufType, GLenum _bufUse);
	~Buffer();
	bool write(int _size, GLvoid* data);
	void* Read(int amount, int offSet);
	GLvoid* read(int amount, int offSet){}
	void bind();
	int inline getSize(){return size;}
private:
	GLuint gBuf;		//the reference to the actual buffer in openGL
	int size;		//contains the size of the buffer in number of bytes. 
	GLenum bufType;		//what type of binding needst to happen.
	GLenum bufUse;		//contians the type that the buffer is. static,stream,dynamic and
					//draw,read,write.
	#ifdef USINGCL
	cl_mem cBuf;		//the reference to the buffer for openCL's concerns. 
				//open cl never got implemented into Delta. this is an artifact.
	#endif
};
#endif
/*.S.D.G.*/
