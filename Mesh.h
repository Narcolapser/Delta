/**************************************************************************************************
 *Program:	Delta
 *Programmer:	Toben "Littlefoot" "Narcolapser" Archer
 *Date:		12 03 09
 *Purpose: 	A wrapper class for mesh objects. With the capacities to load from .obj files and
 *	handle the process of animating and given the values of rotation, transformation, and scale
 *Notes:	HA! I'm making this class again. I must have made this class 4 or 5 times before 
 *	when I had no idea what I was doing. >.<
 **********************/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Attrib.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Mesh
{
public:
	Mesh(Program *prog,const char* filename);
	~Mesh();
	void render();
	void moveRelative(glm::vec3& val);
	void rotateRelative(float by, glm::vec3& val);
	void scaleRelative(glm::vec3& val);
	void resetAll();
	void moveStatic(glm::vec3& val);
	void rotateStatic(float by, glm::vec3& val);
	void scaleStatic(glm::vec3& val);
private:
	//data members:
//	Attrib coords;
	vector<GLfloat> vertices;
	vector<GLushort> elements;
	glm::vec4 loc;
	glm::vec4 rot;
	glm::vec4 scale;
};
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
