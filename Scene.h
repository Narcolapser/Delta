/* This class wraps all of the information for a scene. Meshes, Caameras, materials, etc. 
 * While the resource manager is the class that is responsible for loading classes, for loading all
 * assets, the scene class will be the one that lets the resource manager know what needs to be
 * loaded as the resource manager doesn't do anything on its own.
 */

//#ifndef SCENE_H
//#define SCENE_H
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stack>

#include "Object.cpp"
#include "GeoObject.h"
#include "Camera.h"
#include "ResourceManager.cpp"
#include "Program.cpp"
#include "Model.h"
#include "Trigger.cpp"

using namespace std;
using namespace pugi;

class Scene: public Object //yup, scenes are objects to, they need to be globably adressable.
{
public:
	Scene(const char* config);
	Scene(xml_node config);
	void load(xml_node self, Object* parent, string path);
	void update();//advised to call before calling render.
	void render();
	void bindToProgram(Program *prog, GLint local);
	void bindToProgram(int val);
//private:
	vector<Model*> models;
	vector<Camera*> cameras;
	vector<Program*> programs;
	Camera* viewPoint;
	Program* renderer;
};

//#endif 
/*.S.D.G.*/
