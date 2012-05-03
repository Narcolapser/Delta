#include "Mesh.h"

#ifndef MESH_CPP
#define MESH_CPP

Mesh::Mesh(const char* filename)
{
	//the constructor for the mesh class. This will read in a mesh from a .obj file and send
	//	the information up to the GPU automatically and then maintains the references to
	//	the attributes locally so that a call to "render" can be made simply.

	//open the file. 
	ifstream in(filename, ios::in);
	if (!in) {cout << "Cannot open " << filename << endl;}

	//initalize the variables needed for loading a mesh from an obj.
	vector<GLfloat> verts;
	vector<GLushort> els;
	int vertc = 0;
	int elc = 0;
	string line;

	//loop to read all the data in from the file.
	while (getline(in, line)) //while there is still a line to be had.
	{
		if (line.substr(0,2) == "v ") //if it starts with "v " it is a vertex
		{
			istringstream s(line.substr(2));//read the info in through a stream.
			GLfloat x,y,z;//temp variables to hold the vertex data as it comes in.
			s >> x; s >> y; s >> z;//read in the vertex info
			verts.push_back(x);//push the info onto the vector
			verts.push_back(y);
			verts.push_back(z);
			vertc++;//increment the number of vertices.
		}
		else if (line.substr(0,2) == "f ") //if it starts with "f " it is a face.
		{
			istringstream s(line.substr(2));//stream in again for simplicity.
			GLushort a,b,c,d;//temp variables to hold face info as it comes in.
			s >> a; s >> b; s >> c;//read in the face info.
			a--; b--; c--;// each one needs to be decrememnted because .obj is 1 
				//indexed where as vbos are 0 indexed.
			els.push_back(a);//push info onto the vector
			els.push_back(b);
			els.push_back(c);
			elc++;//incrememnt the number of faces.
			char temp = s.peek();
			if ((int)temp != -1)
			{
				s >> d;
				d--;
				els.push_back(a);
				els.push_back(c);
				els.push_back(d);
				elc++;
			}
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
 	}

	//these two loops convert the vectors into arrays so they can be sent to opengl.
	GLfloat vertarray[vertc*3];
	GLushort elarray[elc*3];

	for(int i = 0; i < vertc; i++)
	{
		vertarray[i*3+0] = verts[i*3+0];
		vertarray[i*3+1] = verts[i*3+1];
		vertarray[i*3+2] = verts[i*3+2];
	}
	for(int i = 0; i < elc; i++)
	{
		elarray[i*3+0] = els[i*3+0];
		elarray[i*3+1] = els[i*3+1];
		elarray[i*3+2] = els[i*3+2];
	}

	//calculate the normals:
	GLfloat vertNorms[vertc*4];
	GLfloat faceNorms[elc*3];
	for(int i = 0; i <elc; i++)
	{
		int vert = elarray[i*3];
		glm::vec3 a(vertarray[vert*3],vertarray[vert*3+1],vertarray[vert*3+2]);

		vert = elarray[i*3+1];
		glm::vec3 b(vertarray[vert*3],vertarray[vert*3+1],vertarray[vert*3+2]);

		vert = elarray[i*3+2];
		glm::vec3 c(vertarray[vert*3],vertarray[vert*3+1],vertarray[vert*3+2]);


		glm::vec3 e1 = a - b;
		glm::vec3 e2 = c - b;
		glm::vec3 normal = glm::core::function::geometric::cross(e1,e2);
		faceNorms[i*3] = normal.x;
		faceNorms[i*3+1] = normal.y;
		faceNorms[i*3+2] = normal.z;
	}

	for(int i = vertc*4; i-->0;vertNorms[i] = 0);
	for(int i = 0; i < elc; i++)
	{
		vertNorms[elarray[i*3]*4] += faceNorms[i*3];
		vertNorms[elarray[i*3]*4+1] += faceNorms[i*3+1];
		vertNorms[elarray[i*3]*4+2] += faceNorms[i*3+2];
		vertNorms[elarray[i*3]*4+3] += 1;

		vertNorms[elarray[i*3+1]*4] += faceNorms[i*3];
		vertNorms[elarray[i*3+1]*4+1] += faceNorms[i*3+1];
		vertNorms[elarray[i*3+1]*4+2] += faceNorms[i*3+2];
		vertNorms[elarray[i*3+1]*4+3] += 1;

		vertNorms[elarray[i*3+2]*4] += faceNorms[i*3];
		vertNorms[elarray[i*3+2]*4+1] += faceNorms[i*3+1];
		vertNorms[elarray[i*3+2]*4+2] += faceNorms[i*3+2];
		vertNorms[elarray[i*3+2]*4+3] += 1;
	}
	GLfloat vNorm[vertc*3];
	for(int i = 0; i < vertc; i++)
	{
		vNorm[i*3] = vertNorms[i*4] / vertNorms[i*4+3];
		vNorm[i*3+1] = vertNorms[i*4+1] / vertNorms[i*4+3];
		vNorm[i*3+2] = vertNorms[i*4+2] / vertNorms[i*4+3];
		GLfloat mag = sqrt(vNorm[i*3]*vNorm[i*3] + vNorm[i*3+1]*vNorm[i*3+1] + vNorm[i*3+2]*vNorm[i*3+2]);
		vNorm[i*3] /= mag;
		vNorm[i*3+1] /= mag;
		vNorm[i*3+2] /= mag;
	}
	//Create Attribute and send data to GPU
	//Binding the buffer objects:

	//send the information from the vertex array to the buffer.
	Buffer* vbo = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vbo->write(sizeof(vertarray),(GLvoid*)vertarray);

	Buffer* norms = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	norms->write(sizeof(vNorm),(GLvoid*)vNorm);

	//same as above.
	elements = new Buffer(GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);
	elements->write(sizeof(elarray),elarray);
	
	//create the attribute objects.
	const char* name = "coord3d";// name of the Attribute of interest.
	coords = new Attrib(name,GL_FLOAT,vbo);
	name = "vNorm";
	normals = new Attrib(name,GL_FLOAT,norms);	
}
Mesh::~Mesh()
{
	delete coords;
	delete elements;
	delete normals;
}
void Mesh::bindToProgram(Program *prog)
{
	coords->bindToProgram(prog);
	normals->bindToProgram(prog);
}
void Mesh::assignID(UID val)
{
	ID = val;
}
void Mesh::render()
{
//	Mesh *cube = this;
	this->coords->enable();
	this->normals->enable();
	this->elements->bind();
	glDrawElements(GL_TRIANGLES, elements->getSize()/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	this->coords->disable();
}


bool Mesh::onEvent(const Event& event)
{
	return false;
}
#endif

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
