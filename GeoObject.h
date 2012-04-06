/* This class is inherits from object and is inherited by classes such as Mesh and Camera. it will
 * contain the information for transforms to be computed locally. The significance of this is that
 * it will allow for parenting of objects. where a local transform can be calculated by taking its
 * parent's transform and multiplying it through. 
 */

#ifndef GEOOBJECT_H
#define GEOOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#include "Object.h"

class GeoObject: public Object
{
public:
	GeoObject()
	{
		loc = glm::vec3(1.0f);
		rotX = 0;
		rotY = 0;
		rotZ = 0;
		trans = glm::mat4(1.0f);
		newTrans = 1;
		parent = NULL;
	}
	void inline setParent(GeoObject* _parent){parent = _parent;}
	glm::mat4 inline getTrans()
	{
		if (newTrans) return trans;
		updateTrans();
		return trans;
	}
	void move(float x, float y, float z)
	{
		if(x != 0 || y != 0 || z != 0)
		{
			loc = glm::vec3(x,y,z);
			newTrans=0;
		}
	}
	void setLoc(glm::vec3 _loc)
	{
		if(loc!=_loc)
		{
			loc = _loc;
			newTrans=0;
		}
	}
	glm::vec3 inline getLoc(){return loc;}
protected:
	glm::mat4 trans;
	glm::vec3 loc;
	float rotX;
	float rotY;
	float rotZ;
	int newTrans;
	GeoObject *parent;
	void updateTrans()
	{
		trans = glm::rotate(glm::mat4(1.0f),rotY,glm::vec3(1.0f,0.0f,0.0f));
		trans = glm::rotate(trans,rotX,glm::vec3(0.0f,1.0f,0.0f));
		trans = glm::rotate(trans,rotZ,glm::vec3(0.0f,0.0f,1.0f));
		trans = glm::translate(trans,loc);
		if (parent) trans = parent->getTrans()*trans;
		newTrans = 1;
	}
};

#endif
