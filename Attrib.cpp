/* this holds data for the InterfaceGL. it holds both the pointer to the buffer
 * object its data is in, as well as the layout of the data information.
 * It contains:
 * Attrib name
 * Instance of Delta's Buffer class
 * number of elements per attribute
 * GL ENUM Type
 * Interweve info.
 * Off set
 */

#include "Attrib.h"

#ifndef ATTRIB_CPP
#define ATTRIB_CPP
Attrib::Attrib(Program *prog, const char* _name, GLenum _type, Buffer* bo)
{//constructor for attrib class. it mostly is just saving the information, it guess some
	canBind = true;
	//of the most common field's values. you can change these later.
	attref = glGetAttribLocation(prog->getProgram(), _name);
	if (attref == -1) 
	{
		fprintf(stderr, "Could not bind attribute %s in the constructor\n", _name);
		canBind = false;
	}	
	name = _name;//name of the attribute.
	type = _type;//what is the type of data stored in it? GLfloat, GLint, etc.
	numEl = 3;//the number of elements. assumed 3, as 3 is normal for 3D games.
	interweave = 0;//assumes that there is no interweave on these arrays.
	offset = 0;//starting the data right at the begining. again, can be changed later.
	buffer = bo;//the reference to the actual buffer object.
	
}
Attrib::Attrib(const char* _name, GLenum _type, Buffer* bo)
{
	canBind = false;
	name = _name;//name of the attribute.
	type = _type;//what is the type of data stored in it? GLfloat, GLint, etc.
	numEl = 3;//the number of elements. assumed 3, as 3 is normal for 3D games.
	interweave = 0;//assumes that there is no interweave on these arrays.
	offset = 0;//starting the data right at the begining. again, can be changed later.
	buffer = bo;//the reference to the actual buffer object.
}
void Attrib::bindToProgram(Program *prog)
{
	canBind = true;
	//of the most common field's values. you can change these later.
	attref = glGetAttribLocation(prog->getProgram(), name);
	if (attref == -1) 
	{
		fprintf(stderr, "Could not bind attribute %s post constructor\n", name);
		canBind = false;
	}
}
Attrib::~Attrib()
{//deconstructor. all that needs to be deleted is the name and the buffere freed.
	delete name;
	delete buffer;
}
void inline Attrib::enable()
{//enable the buffer.
	buffer->bind();
	glEnableVertexAttribArray(attref);
	pointer();
}
void inline Attrib::pointer()
{//tie the attribute location to here
	glVertexAttribPointer(
		attref,			// attribute
		numEl,			// number of elements per vertex
		type,			// the type of each element
		GL_FALSE,		// take the values as-is
		interweave,		// no extra data between each position
		(GLvoid*) offset	// offset of first element
		);
}
void inline Attrib::disable()
{//disable the buffer now that things are done.
	glDisableVertexAttribArray(attref);
}
//void inline setBuffer(GLuint val) { buffer = val; } // change the buffer. 
	//you shouldn't use this really. i'll probably delete it later.
void inline Attrib::setNumEl(int val) { numEl = val; }//change the number of elements in the
	//attribute from the default of 3.
void inline Attrib::setInterweave(int val) { interweave = val; } //set an interweave value.
void inline Attrib::setOffset(long val) { offset = (GLvoid*) val; } //set an offset, usually
	//used in conjunction with interweave. and by usually, i mean basically only.
//char* inline getName() {return name;}//this never worked.
GLint inline Attrib::getAttref() {return attref;} //get the reference to the attribute, another
	//peice that is still in to fill gaps but I hope to remove later.
//GLuint inline getBuffer() { return buffer;}
	//get the buffer. this can be handy when more than one attribute needs to share it.
int inline Attrib::getNumEl() {return numEl;} //get the current number of elements per vertex
GLenum inline Attrib::getType() {return type;} //get the type the buffer is.
int inline Attrib::getInterweave() {return interweave;} //get the current interweave
long inline Attrib::getOffset() {return (long)offset;} //get the current offset.
#endif
/*.S.D.G.*/
