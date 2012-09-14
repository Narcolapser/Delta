/* this holds data for the open GL attributes. it holds both the pointer to the buffer
 * object its data is in, as well as the layout of the data information.
 * It contains:
 * Attrib name
 * Instance of Delta's Buffer class
 * number of elements per attribute
 * GL ENUM Type
 * Interweve info.
 * Off set
 */

#include "Program.cpp"
#include "Buffer.cpp"

#ifndef ATTRIB_H
#define ATTRIB_H
class Attrib
{
public:
	Attrib(Program *prog, const char* _name, GLenum _type, Buffer* bo);
	Attrib(const char* _name, GLenum _type, Buffer* bo);
	void bindToProgram(Program *prog);
	~Attrib();
	void inline enable();
	void inline pointer();
	void inline disable();
	void inline setNumEl(int val);
	void inline setInterweave(int val);
	void inline setOffset(long val);
	GLint inline getAttref();
	int inline getNumEl();
	GLenum inline getType();
	int inline getInterweave();
	long inline getOffset();
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
