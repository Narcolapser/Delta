/* this holds data for interfacing with the graphics card, both from OpenGl and OpenCL. These data
 * are held in buffered objects. Buffer objects are also how OpenGL and OpenCL share information
 * and as such this class will also be responsible for creating the interopability between them on
 * these buffer objects. 
 * It contains:
 * GL/CL Buffer object reference
 * size of buffer
 */

#ifndef BUFFER_CPP
#define BUFFER_CPP

#include "Buffer.h"

Buffer::Buffer(GLenum _bufType, GLenum _bufUse)
{
	glGenBuffers(1, &gBuf);	//create the buffer on the GL side.
	size = 0;		//save the size of the buffer
	bufType = _bufType;	//save the type of buffer for binding later.
	bufUse = _bufUse;	//how will the buffer be used
}
Buffer::~Buffer()
{
	glDeleteBuffers(1, &gBuf);
	gBuf = 0;
	size = 0;
}
bool Buffer::write(int _size, GLvoid* data)
{
	size = _size;
	glBindBuffer(bufType, gBuf);//bind the buffer so it can be worked on.
	glBufferData(bufType, size, data, bufUse);
	return true;
}
void* Buffer::Read(int amount, int offSet)
{
	return (void*)read(amount,offSet);
}
void Buffer::bind()
{//bind it so it may be used
	glBindBuffer(bufType, gBuf);
}

#endif
/*.S.D.G.*/
