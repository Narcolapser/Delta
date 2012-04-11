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

#include "Object.h"
#include "GeoObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "ResourceManager.cpp"
#include "Program.h"
#include "lib/pugixml/pugixml.cpp"

using namespace std;
using namespace pugi;

class Scene: public Object //yup, scenes are objects to, they need to be globably adressable.
{
public:
	Scene(const char* config);
	Scene(xml_node config);
	void update();//advised to call before calling render.
	void render();
	void bindToProgram(Program *prog, GLint local);
//private:
	vector<Mesh*> meshes;
	vector<Camera*> cameras;
};

//#endif 
/*.S.D.G.*/
