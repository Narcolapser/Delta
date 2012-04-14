/**************************************************************************************************
 *Program:	Delta
 *Programmer:	Toben "Littlefoot" "Narcolapser" Archer
 *Date:		12 04 14
 *Purpose: 	A class for managing the display of geometry.
 **********************/

#include "ResourceManager.cpp"
#include "Mesh.cpp"
#include "GeoObject.h"
#include "Program.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model: public GeoObject
{
public:
	Model(UID _meshID)
	{
		mesh = (Mesh*)globalRM->GetIDRetaining(_meshID);
		meshID = _meshID;
		local = 0;
		ID = globalRM->RequestID();
	}
	~Model()
	{
		mesh = NULL;
		globalRM->Release(meshID);
		meshID = 0;
		local = 0;
	}
	void bindToProgram(Program* prog, GLint _local)
	{
		mesh->bindToProgram(prog);
		local = _local;
	}
	void render()
	{
		glUniformMatrix4fv(local, 1, GL_FALSE, glm::value_ptr(getTrans()));
		mesh->render();
	}
private:
	Mesh* mesh;
	UID meshID;
	GLint local;
};

/*.S.D.G.*/
