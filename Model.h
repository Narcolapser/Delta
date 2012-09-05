/**************************************************************************************************
 *Program:	Delta
 *Programmer:	Toben "Littlefoot" "Narcolapser" Archer
 *Date:		12 04 14
 *Purpose: 	A class for managing the display of geometry.
 **********************/

#include "ResourceManager.cpp"
#include "Mesh.cpp"
#include "GeoObject.h"
#include "Program.cpp"
#include <stdio.h>
#include <iostream>
#include <sstream>

#ifndef MODEL_H
#define MODEL_H

class Model: public GeoObject
{
public:
	Model(UID _meshID);
	Model(UID _meshID, float x, float y, float z);
	Model(xml_node& self, string path);
	~Model();
	void bindToProgram(Program* prog, GLint _local);
	void render();
	virtual bool onEvent(const Event& event);
private:
	Mesh* mesh;
	UID meshID;
	GLint local;
};

/*.S.D.G.*/
#endif
