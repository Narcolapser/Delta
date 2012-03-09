/* this holds data for the InterfaceGL. it holds both the pointer to the buffer
 * object its data is in, as well as the layout of the data information.
 * It contains:
 * Attrib name
 * GL Buffer object reference
 * number of elements per attribute
 * GL ENUM Type
 * Interweve info.
 * Off set
 */

#include "Program.h"

class Attrib
{
public:
	Attrib(Program *prog, const char* _name, GLenum _type, GLuint bo)
	{
		attref = glGetAttribLocation(prog->getProgram(), _name);
		if (attref == -1) 
		{
			fprintf(stderr, "Could not bind attribute %s\n", _name);
		}
		name = _name;
		type = _type;
		numEl = 3;
		interweave = 0;
		offset = 0;
		buffer = bo;
		
	}
	~Attrib()
	{
		delete name;
	}
	void inline enable()
	{
		glEnableVertexAttribArray(attref);
	}
	void inline bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);	
	}
	void inline pointer()
	{
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
	{
		glDisableVertexAttribArray(attref);
	}
	void inline setBuffer(GLuint val) { buffer = val; }
	void inline setNumEl(int val) { numEl = val; }
	void inline setInterweave(int val) { interweave = val; }
	void inline setOffset(long val) { offset = (GLvoid*) val; }

//	char* inline getName() {return name;}
	GLint inline getAttref() {return attref;}
	GLuint inline getBuffer() { return buffer;}
	int inline getNumEl() {return numEl;}
	GLenum inline getType() {return type;}
	int inline getInterweave() {return interweave;}
	long inline getOffset() {return (long)offset;}
private:
	//data members
	const char* name;
	GLint attref;
	GLuint buffer;
	int numEl;
	GLenum type;
	int interweave;
	GLvoid* offset;	
};
