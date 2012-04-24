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
	GLvoid* read(int amount, int offSet);
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
	#endif
};
#endif
/*.S.D.G.*/


/*#ifndef BUFFER_H
#define BUFFER_H

#include "Object.cpp"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
/*
using namespace std;

class Buffer: public Object
{
public:
	Buffer(GLenum _bufType, GLenum _bufUse)
	{
		glGenBuffers(1, &gBuf);	//create the buffer on the GL side.
		size = 0;		//save the size of the buffer
		bufType = _bufType;	//save the type of buffer for binding later.
		bufUse = _bufUse;	//how will the buffer be used
	}
	~Buffer()
	{
		glDeleteBuffers(1, &gBuf);
		gBuf = 0;
		size = 0;
	}
//	bool inline Write(void* data)
//	{
//		return write((GLvoid*) data);
//	}
	bool write(int _size, GLvoid* data)
	{
		size = _size;
		glBindBuffer(bufType, gBuf);//bind the buffer so it can be worked on.
		glBufferData(bufType, size, data, bufUse);
		return true;
	}
	void* Read(int amount, int offSet)
	{
		return (void*)read(amount,offSet);
	}
	GLvoid* read(int amount, int offSet)
	{
		
	}
	void inline bind()
	{//bind it so it may be used
//		printf("%i size: %i\n",gBuf,size);
		glBindBuffer(bufType, gBuf);
	}
	int inline getSize(){return size;}
private:
	GLuint gBuf;		//the reference to the actual buffer in openGL
	int size;		//contains the size of the buffer in number of bytes. 
	GLenum bufType;		//what type of binding needst to happen.
	GLenum bufUse;		//contians the type that the buffer is. static,stream,dynamic and
					//draw,read,write.
	#ifdef USINGCL
	cl_mem cBuf;		//the reference to the buffer for openCL's concerns. 
	#endif
};
#endif
/*.S.D.G.*/
