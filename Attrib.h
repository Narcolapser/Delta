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

#include "Program.h"
#include "Buffer.h"

#ifndef ATTRIB_H
#define ATTRIB_H
class Attrib
{
public:
	Attrib(Program *prog, const char* _name, GLenum _type, Buffer* bo)
	{//constructor for attrib class. it mostly is just saving the information, it guess some
		canBind = true;
		//of the most common field's values. you can change these later.
		attref = glGetAttribLocation(prog->getProgram(), _name);
		if (attref == -1) 
		{
			fprintf(stderr, "Could not bind attribute %s\n", _name);
			canBind = false;
		}	
		name = _name;//name of the attribute.
		type = _type;//what is the type of data stored in it? GLfloat, GLint, etc.
		numEl = 3;//the number of elements. assumed 3, as 3 is normal for 3D games.
		interweave = 0;//assumes that there is no interweave on these arrays.
		offset = 0;//starting the data right at the begining. again, can be changed later.
		buffer = bo;//the reference to the actual buffer object.
		
	}
	Attrib(const char* _name, GLenum _type, Buffer* bo)
	{
		canBind = false;
		name = _name;//name of the attribute.
		type = _type;//what is the type of data stored in it? GLfloat, GLint, etc.
		numEl = 3;//the number of elements. assumed 3, as 3 is normal for 3D games.
		interweave = 0;//assumes that there is no interweave on these arrays.
		offset = 0;//starting the data right at the begining. again, can be changed later.
		buffer = bo;//the reference to the actual buffer object.
	}
	void bindToProgram(Program *prog)
	{
		canBind = true;
		//of the most common field's values. you can change these later.
		attref = glGetAttribLocation(prog->getProgram(), name);
		if (attref == -1) 
		{
			fprintf(stderr, "Could not bind attribute %s\n", name);
			canBind = false;
		}
	}
	~Attrib()
	{//deconstructor. all that needs to be deleted is the name and the buffere freed.
		delete name;
		delete buffer;
	}
	void inline enable()
	{//enable the buffer.
		buffer->bind();
		glEnableVertexAttribArray(attref);
		pointer();
	}

	void inline pointer()
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
	void inline disable()
	{//disable the buffer now that things are done.
		glDisableVertexAttribArray(attref);
	}

//	void inline setBuffer(GLuint val) { buffer = val; } // change the buffer. 
		//you shouldn't use this really. i'll probably delete it later.
	void inline setNumEl(int val) { numEl = val; }//change the number of elements in the
		//attribute from the default of 3.
	void inline setInterweave(int val) { interweave = val; } //set an interweave value.
	void inline setOffset(long val) { offset = (GLvoid*) val; } //set an offset, usually
		//used in conjunction with interweave. and by usually, i mean basically only.

//	char* inline getName() {return name;}//this never worked.
	GLint inline getAttref() {return attref;} //get the reference to the attribute, another
		//peice that is still in to fill gaps but I hope to remove later.
//	GLuint inline getBuffer() { return buffer;}
		//get the buffer. this can be handy when more than one attribute needs to share it.
	int inline getNumEl() {return numEl;} //get the current number of elements per vertex
	GLenum inline getType() {return type;} //get the type the buffer is.
	int inline getInterweave() {return interweave;} //get the current interweave
	long inline getOffset() {return (long)offset;} //get the current offset.
private:
	//data members
	const char* name;	//name of the attribute of concern.
	bool canBind;		//a value to keep whether or not this attribute is useable.
	GLint attref;		//the gl reference to the attribute.
	Buffer* buffer;		//the buffer in use.
	int numEl;		//the number of elements per vertex.
	GLenum type;		//the type of data stored in the buffer.
	int interweave;		//the amount of inter weave.
	GLvoid* offset;		//the offset to the first element.
};

#endif

/*.S.D.G.*/
