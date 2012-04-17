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
#include <glm/gtc/quaternion.hpp>

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

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
		newTrans = clock();
		parent = NULL;
		ptranC = 0;
		tranC = 0;
	}
	void inline setParent(GeoObject* _parent){parent = _parent;}
	long int inline getTranC(){return tranC;}
	glm::mat4 getTrans()
	{
//		if (parent)
//		{
//			if(ptranC < parent->getTranC())
//			{
//				newTrans = false;
//				printf("ptranC: %li parent->tranC: %li\n",ptranC,parent->getTranC());
//			}
//			ptranC = parent->getTranC();
//		}			
//		if (newTrans) return trans;
		updateTrans();
		return trans;
	}
	void move(float x, float y, float z)
	{
		if(!(x == 0 && y == 0 && z == 0))
		{
			loc = loc + glm::vec3(x,y,z);
			newTrans=false;
		}
	}
	void rotate(float _x, float _y, float _z)
	{
		if(!(_x == 0 && _y == 0 && _z == 0))
		{
			newTrans=false;
//			rotX += x;
//			rotY += y;
//			rotZ += z;

		float p = _y * 0.00872664626;
		float y = _z * 0.00872664626;
		float r = _x * 0.00872664626;

		float sinp = sin(p);
		float siny = sin(y);
		float sinr = sin(r);
		float cosp = cos(p);
		float cosy = cos(y);
		float cosr = cos(r);

		float qx = sinr * cosp * cosy - cosr * sinp * siny;
		float qy = cosr * sinp * cosy + sinr * cosp * siny;
		float qz = cosr * cosp * siny - sinr * sinp * cosy;
		float qw = cosr * cosp * cosy + sinr * sinp * siny;
		
		glm::gtc::quaternion::quat local = glm::gtc::quaternion::normalize(
			glm::gtc::quaternion::quat(qw,qx,qy,qz));
		
		rotquat = local * rotquat;
		}
	}
	void setLoc(glm::vec3 _loc)
	{
		if(loc!=_loc)
		{
			loc = _loc;
			newTrans=false;
		}
	}
	void setRot(float _x, float _y, float _z)
	{//this was a little clever idea I had to remove a conditional. rather than having an if
	//statement being what set the newTrans to false. what thought was I could just simply
	//compare the current rotation on each axis and set newTrans to that. if the values are the
	//same than nother really happens when the values get set. but setting it is a lot faster 
	//than trying to deal with the conditionals. but how do I deal with the problem of x and/or
	//y being different and z being the same? well, voodoo magic of course! if I put an and
	//statement into the comparison, if newTrans was false, the whole thing will be evaluated
	//as false. further, if one higher up evaluates to false at any point, then the logic can
	//short circuit and no comparison need be made. 
		newTrans = newTrans && rotX == _x;
		rotX = _x;
		newTrans = newTrans && rotY == _y;
		rotY = _y;
		newTrans = newTrans && rotZ == _z;
		rotZ = _z;

		float p = rotX * 0.00872664626;
		float y = rotY * 0.00872664626;
		float r = rotZ * 0.00872664626;

		float sinp = sin(p);
		float siny = sin(y);
		float sinr = sin(r);
		float cosp = cos(p);
		float cosy = cos(y);
		float cosr = cos(r);

		float qx = sinr * cosp * cosy - cosr * sinp * siny;
		float qy = cosr * sinp * cosy + sinr * cosp * siny;
		float qz = cosr * cosp * siny - sinr * sinp * cosy;
		float qw = cosr * cosp * cosy + sinr * sinp * siny;

		rotquat = glm::gtc::quaternion::normalize(glm::gtc::quaternion::quat(qw,qx,qy,qz));
	}
	glm::vec3 inline getLoc(){return loc;}
protected:
	glm::mat4 trans;
	glm::vec3 loc;
	float rotX;
	float rotY;
	float rotZ;
	bool newTrans;
	long int tranC;
	GeoObject *parent;
	long int ptranC;
	glm::gtc::quaternion::quat rotquat;
	void updateTrans()
	{
//		trans = glm::rotate(glm::mat4(1.0f),rotY,glm::vec3(1.0f,0.0f,0.0f));
//		trans = glm::rotate(trans,rotX,glm::vec3(0.0f,1.0f,0.0f));
//		trans = glm::rotate(trans,rotZ,glm::vec3(0.0f,0.0f,1.0f));
//		trans = glm::rotate(glm::mat4(1.0f),1.0f,glm::vec3(rotX,rotY,rotZ));

//		glm::gtc::quaternion::quat xq(cos(rotX * 0.00872664626), 
//				glm::vec3(sin(rotX * 0.00872664626),0.0,0.0));
//		glm::gtc::quaternion::quat yq(cos(rotY * 0.00872664626), 
//				glm::vec3(0.0,sin(rotY * 0.00872664626),0.0));
//		glm::gtc::quaternion::quat zq(cos(rotZ * 0.00872664626), 
//				glm::vec3(0.0,0.0,sin(rotZ * 0.00872664626)));
//		glm::gtc::quaternion::quat rotquat = glm::gtc::quaternion::normalize(xq * yq * zq);

		trans = glm::gtc::quaternion::mat4_cast(rotquat);
		trans = glm::translate(trans,loc);
		if (parent) trans = parent->getTrans() * trans;
		newTrans = true;
		++tranC;
	}
};

/*.S.D.G.*/
#endif
