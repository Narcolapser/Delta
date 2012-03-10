#include "Mesh.h"

Mesh::Mesh(Program *prog, const char* filename)
{

	ifstream in(filename, ios::in);

	if (!in) {cerr << "Cannot open " << filename << endl;}
	
	GLfloat *verts = NULL;
	GLushort *els = NULL;

	verts = (GLfloat*) realloc (3000, count * sizeof(GLfloat));
	els = (GLushort*) realloc (3000, count * sizeof(GLushort));

	int vertc = 0;
	int elc = 0;
	string line;
	while (getline(in, line)) 
	{

		if (line.substr(0,2) == "v ") 
		{
			if(vertc == 1000)
			{
				verts = (GLfloat*) realloc (65535*3, count * sizeof(GLfloat));
			}
			istringstream s(line.substr(2));
			s >> verts[vertc*3]; 
			s >> verts[vertc*3+1]; 
			s >> verts[vertc*3+2];		
			vertc++;
		}
		else if (line.substr(0,2) == "f ") 
		{
			if(elc == 1000)
			{
				els = (GLushort*) realloc (65535, count * sizeof(GLushort));
			}
			istringstream s(line.substr(2));
			GLushort a,b,c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			els[elc*3] = a;
			els[elc*3+1] = b;
			els[elc*3+2] = c;
			elc++;
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
 	}

	verts = (GLfloat*) realloc (vertc*3, count * sizeof(GLfloat));
	els = (GLushort*) realloc (elc*3, count * sizeof(GLushort));

	//Create Attribute and send data to GPU
	const char* name = "coord3d";

//	coords = new Attrib(prog,name,GL_Float,
}
Mesh::~Mesh(){}
void Mesh::render(){}
void Mesh::moveRelative(glm::vec3& val){}
void Mesh::rotateRelative(float by, glm::vec3& val){}
void Mesh::scaleRelative(glm::vec3& val){}
void Mesh::resetAll(){}
void Mesh::moveStatic(glm::vec3& val){}
void Mesh::rotateStatic(float by, glm::vec3& val){}
void Mesh::scaleStatic(glm::vec3& val){}


//	Attrib coords;
//	vector<GLfloat> vertices;
//	vector<GLushort> elements;
//	glm::vec4 loc;
//	glm::vec4 rot;
//	glm::vec4 scale;

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

int main(int argc, char* argv[])
{
	return 0;
}
