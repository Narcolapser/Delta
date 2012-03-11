#include "Mesh.h"

Mesh::Mesh(Program *prog, const char* filename)
{
//	cout <<"I started" << endl;
	ifstream in(filename, ios::in);

	if (!in) {cout << "Cannot open " << filename << endl;}
	
//	GLfloat *verts = NULL;
//	GLushort *els = NULL;

//	cout <<"File opened, got pointers." << endl;

	GLfloat verts[65535];
	GLushort els[65535];

//	verts = (GLfloat*) realloc (verts, 3000 * sizeof(GLfloat));
//	els = (GLushort*) realloc (els, 3000 * sizeof(GLushort));

//	verts = (GLfloat*) realloc (verts, 3000 );
//	els = (GLushort*) realloc (els, 3000 );

	int vertc = 0;
	int elc = 0;
	string line;
//	cout << "allocated. verts is now:" << sizeof(verts) << " vertc is: " << vertc << " els is now: " << sizeof(els) << " elc is: " << elc << endl;
	while (getline(in, line)) 
	{

		if (line.substr(0,2) == "v ") 
		{
//			if(vertc == 1000)
//			{
//				verts = (GLfloat*) realloc (verts, 65535 * sizeof(GLfloat));
//			}
			istringstream s(line.substr(2));
			s >> verts[vertc*3];
			s >> verts[vertc*3+1];
			s >> verts[vertc*3+2];
//			cout << "vert: " << vertc << " values: " << verts[vertc] << " : " << verts[vertc+1] << " : " << verts[vertc+2] << endl;
			vertc++;			
		}
		else if (line.substr(0,2) == "f ") 
		{
//			if(elc == 1000)
//			{
//				els = (GLushort*) realloc (els, 65535 * sizeof(GLushort));
//			}
			istringstream s(line.substr(2));
			GLushort a,b,c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			els[elc*3] = a;
			els[elc*3+1] = b;
			els[elc*3+2] = c;
//			cout << "element: " << elc << " values: " << els[elc] << " : " << els[elc+1] << " : " << els[elc+2] << endl;
			elc++;
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
 	}

//	cout << "done with while." << endl;
//	verts = (GLfloat*) realloc (verts, vertc * 3 * sizeof(GLfloat));
//	els = (GLushort*) realloc (els, elc * 3 * sizeof(GLushort));

//	verts = (GLfloat*) realloc (verts, 96);
//	els = (GLushort*) realloc (els, 60);

//	cout << "reallocated. verts is now:" << sizeof(verts) << " vertc is: " << vertc << " els is now: " << sizeof(els) << " elc is: " << elc << endl;
	//Create Attribute and send data to GPU
	const char* name = "coord3d";

	//Binding the buffer objects:
	GLuint vbo_verts, ibo_elements;

	glGenBuffers(1, &vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(els), els, GL_STATIC_DRAW);
//	cout << "time to make attributes." << endl;
	coords = new Attrib(prog,name,GL_FLOAT,vbo_verts,GL_ARRAY_BUFFER);
	elements = new Attrib(prog,name,GL_FLOAT,ibo_elements,GL_ELEMENT_ARRAY_BUFFER);
	
//	free(verts);
//	free(els);
//	verts = NULL;
//	els = NULL;

	loc = glm::mat4(1.0f);
	rot = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);
	trans = glm::mat4(1.0f);
	newTrans = 1;
//	cout << "donzies!" << endl;
}
Mesh::~Mesh()
{
	delete coords;
	delete elements;
	loc = glm::mat4(0.0f);
	rot = glm::mat4(0.0f);
	scale = glm::mat4(0.0f);
	trans = glm::mat4(0.0f);
	newTrans = 0;
}
void Mesh::render(GLint local)
{
//	if(newTrans != 1) 
//	{
//		trans = glm::mat4(1.0f) * scale;
//		trans = trans * rot;
//		trans = trans * loc;
//		newTrans == 1;
//	}
//	glUniformMatrix4fv(local, 1, GL_FALSE, glm::value_ptr(trans));
	
	int size;
//	coords->enable();
//	coords->bind();
//	coords->pointer();
//	elements->bind();
//	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
//	coords->disable();

	Mesh *cube = this;
	cube->coords->enable();
	cube->coords->bind();
	cube->coords->pointer();
	cube->elements->bind();
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	cube->coords->disable();

}
void Mesh::setTrans(glm::mat4 val)
{
	trans = val;
	newTrans = 1;
}
void Mesh::moveRelative(glm::vec3& val){}
void Mesh::rotateRelative(float by, glm::vec3& val){}
void Mesh::scaleRelative(glm::vec3& val){}
void Mesh::resetAll(){}
void Mesh::moveStatic(glm::vec3& val){}
void Mesh::rotateStatic(float by, glm::vec3& val){}
void Mesh::scaleStatic(glm::vec3& val){}


//	Attrib coords;
//	Attrib elements;
//	glm::mat4 loc;
//	glm::mat4 rot;
//	glm::mat4 scale;
//	glm::mat4 trans;
//	int newTrans;

/*.S.D.G.*/

/*little references:

    glm::vec4 v(0);
    glm::mat4 m(0);
    glVertex3fv(glm::value_ptr(v));
    glLoadMatrixfv(glm::value_ptr(m)); 

    glm::vec4 v(0);
    glm::mat4 m(0);
    glVertex3fv(&v[0]);
    glLoadMatrixfv(&m[0][0]); 
*/
