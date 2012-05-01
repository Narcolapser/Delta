#include "Scene.h"
Scene::Scene(xml_node config)
{//constructor for scene. takes an xml_node with the configuration fo this scene.
	//read in the path so that the location of the resources are known.
	string path = config.attribute("path").value() + string("/");

	for(xml_node i = config.first_child(); i; i = i.next_sibling())
	{//itterate over all the objects in the scene. pulling each one in with no parent.
		load(i,NULL,path);
	}
}

void Scene::load(xml_node self, Object* parent, string path)
{//this method is a recursive system for loading the scenes. each time the parenting relationship
	//goes another step deeper, the recusion does too.

	//first off, find out what type of object this is. this will read what it is in from the
	//attribute and translate the string into the delta enumerated type.
	delta_t name = stringToEnum(self.name());

	//temporary holder for the instantiation of the new object.
	Object* temp;
	switch(name)
	{//switch over all the possibilities. if the object type is one that isn't known, ignore it
		//the rest of this is fairly straight forward. create a new whatever object, push
		//it on to the vector of the appropriate type. 
		//if this is the top level, parent is null, so setting parent does nothing. if this isn't
		//the top level, this will parent this current object to its parent.
	case MODEL:
		temp = new Model(self,path);
		models.push_back((Model*)temp);
		((GeoObject*)temp)->setParent((GeoObject*)parent);
		break;
	case CAMERA:
		temp = new Camera(self);
		cameras.push_back((Camera*)temp);
		((GeoObject*)temp)->setParent((GeoObject*)parent);
		break;
	case PROGRAM:
		temp = new Program(self,path);
		programs.push_back((Program*)temp);
		globalRM->AssignID(globalRM->RequestID(),temp);
		break;
	default:
		break;
	}
	//finally loop through and load all of this object's children. if it doesn't have any, this
	//loop never happens. handy no?
	for(xml_node i = self.first_child(); i; i = i.next_sibling())
	{
		load(i,temp,path);
	}
}

void Scene::render()
{
	if(renderer==NULL) bindToProgram(0);
	if(viewPoint == NULL) viewPoint = cameras[0];
	renderer->use();
	glUniformMatrix4fv(renderer->getVP(), 1, GL_FALSE,
		glm::value_ptr((viewPoint->getProjection() * viewPoint->view())));
	//this loops through and renders each mesh that is in the vector. I might add a system for
	//checking to see if the object is currently flagged to render or not, but for now this
	//just gets things onto the screen which is what matters.
	for(int i = 0; i < models.size(); i++)
	{
		models[i]->render();
	}
}
void Scene::bindToProgram(Program *prog, GLint local)
{
	//bind the models to the appropriate program. I could have done this in the constructor,
	//but this allows for the program to be changed and added later if nescessary. makes things
	//easier over all. it just itterates through the models and calls them to bind to prog.
	for(int i = 0; i < models.size(); i++)
	{
		models[i]->bindToProgram(prog,local);
	}
}
void Scene::bindToProgram(int val)
{
	renderer = programs[val];
	GLint local = renderer->getLocal();
	//bind the models to a locally saved program. I could have done this in the constructor,
	//but this allows for the program to be changed and added later if nescessary. makes things
	//easier over all. it just itterates through the models and calls them to bind to prog.
	for(int i = 0; i < models.size(); i++)
	{
		models[i]->bindToProgram(renderer,local);
	}
}
/*.S.D.G.*/
