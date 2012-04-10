/* This class's purpose is to deal with the loading and deletion of files for the rest of the
 * program. In the future this will allow me to make redundancy checks and that sort of thing. But
 * for now, I've got this as a small first step. 
 * This class contains:
 */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>

#include "Buffer.h"
#include "Attrib.h"
#include "Mesh.h"

#include "lib/pugixml/pugixml.hpp"

using namespace std;

class ResourceManager
{
public:
	ResourceManager(const char* config, bool isFile);
	Mesh* loadMesh(const char* fName);
private:
};

#endif
/*.S.D.G.*/
