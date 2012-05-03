/* This class is inherits from object and is inherited by classes such as Mesh and Camera. it will
 * contain the information for transforms to be computed locally. The significance of this is that
 * it will allow for parenting of objects. where a local transform can be calculated by taking its
 * parent's transform and multiplying it through. 
 */


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


#include "Object.cpp"

using namespace std;

class GeoObject: public Object
{
public:
	GeoObject();
	void inline setParent(GeoObject* _parent){parent = _parent;}
	long int inline getTranC(){return tranC;}
	glm::mat4 getTrans();
	glm::vec3 getGlobalLoc();
	void move(float x, float y, float z);
	void rotate(float _x, float _y, float _z);
	void setLoc(glm::vec3 _loc);
	void setRot(float _x, float _y, float _z);
	glm::vec3 inline getLoc(){return loc;}
	virtual bool onEvent(const Event& event);
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
	void updateTrans();
};

/*.S.D.G.*/
#endif
