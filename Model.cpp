/**************************************************************************************************
 *Program:	Delta
 *Programmer:	Toben "Littlefoot" "Narcolapser" Archer
 *Date:		12 04 14
 *Purpose: 	A class for managing the display of geometry.
 **********************/

#include "Model.h"

#ifndef MODEL_CPP
#define MODEL_CPP

Model::Model(UID _meshID)
{//constructor when the mesh is already known to be loaded. 
	mesh = (Mesh*)globalRM->GetIDRetaining(_meshID);
	meshID = _meshID;
	local = 0;
	ID = globalRM->RequestID();
}
Model::Model(xml_node& self, string path)
{
//	printf("Loading: %s\n",(path + self.attribute("path").value()).c_str());
	//load the mesh, but do so through the resource manager to prevent duplicates.
	mesh = (Mesh*)globalRM->GetIDNonRetaining(
			globalRM->LoadMesh((path + self.attribute("path").value()).c_str()));
	//move it according to what is given in the xml.
	move(self.attribute("locX").as_float(),
		self.attribute("locY").as_float(),
		self.attribute("locZ").as_float());
	//since the location of the local transform for the shaders isn't known yet, set it
	//to zero until it is known.
	local = 0;
	//request a newID from the resource manager.
	ID = globalRM->RequestID();
	//then this object assigns itself to that ID.
	globalRM->AssignID(ID,this);
}
Model::~Model()
{//delete the model. release its ID and set mesh to a null pointer. DO NOT ELETE MESH!
	//let globalRM deal with the deletion, just nullifie the local pointer.
	mesh = NULL;
	//tell globalRM that the object is not being used by you any more.
	globalRM->Release(meshID);
	meshID = 0;
	local = 0;
}
void Model::bindToProgram(Program* prog, GLint _local)
{//bind the model to a program. to do this the local uniform must be known as ell as
	//what the program is. the later is simply passed through to the nderlying mesh.
	mesh->bindToProgram(prog);
	local = _local;
}
void Model::render()
{//render this model!
	glUniformMatrix4fv(local, 1, GL_FALSE, glm::value_ptr(getTrans()));
	mesh->render();
}
bool Model::onEvent(const Event& event)
{
	return GeoObject::onEvent(event);
}

/*.S.D.G.*/
#endif
