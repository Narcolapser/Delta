#include "Mesh.h"

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

	//Create Attribute and send data to GPU
	//Binding the buffer objects:

	//send the information from the vertex array to the buffer.
	Buffer* vbo = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vbo->write(sizeof(vertarray),(GLvoid*)vertarray);

	//same as above.
	elements = new Buffer(GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);
	elements->write(sizeof(elarray),elarray);
	
	//create the attribute objects.
	const char* name = "coord3d";// name of the Attribute of interest.
	coords = new Attrib(name,GL_FLOAT,vbo);
	
	local = 0;
	//set local values to defaults.
//	loc = glm::mat4(1.0f);
//	rot = glm::mat4(1.0f);
//	scale = glm::mat4(1.0f);
//	trans = glm::mat4(1.0f);
//	newTrans = 1;
}
Mesh::~Mesh()
{
	delete coords;
	delete elements;
//	loc = glm::mat4(0.0f);
//	rot = glm::mat4(0.0f);
//	scale = glm::mat4(0.0f);
//	trans = glm::mat4(0.0f);
//	newTrans = 0;
}
void Mesh::bindToProg(Program *prog, GLint _local)
{
	coords->bindToProg(prog);
	local = local;
}
void Mesh::render()
{
	glUniformMatrix4fv(local, 1, GL_FALSE, glm::value_ptr(getTrans()));
	Mesh *cube = this;
	this->coords->enable();
	this->elements->bind();
	glDrawElements(GL_TRIANGLES, elements->getSize()/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	this->coords->disable();
}
void Mesh::setTrans(glm::mat4 val)
{
	trans = val;
	newTrans = 1;
}
//void Mesh::moveRelative(glm::vec3& val){}
//void Mesh::rotateRelative(float by, glm::vec3& val){}
//void Mesh::scaleRelative(glm::vec3& val){}
//void Mesh::resetAll(){}
//void Mesh::moveStatic(glm::vec3& val){}
//void Mesh::rotateStatic(float by, glm::vec3& val){}
//void Mesh::scaleStatic(glm::vec3& val){}


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
