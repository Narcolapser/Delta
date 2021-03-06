/* This class is inherits from object and is inherited by classes such as Mesh and Camera. it will
 * contain the information for transforms to be computed locally. The significance of this is that
 * it will allow for parenting of objects. where a local transform can be calculated by taking its
 * parent's transform and multiplying it through. 
 */

#ifndef GEOOBJECT_CPP
#define GEOOBJECT_CPP
#include "GeoObject.h"

GeoObject::GeoObject()
{//default constructor for geoobject.
	loc = glm::vec3(0.0f);
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	trans = glm::mat4(1.0f);
	newTrans = clock();
	parent = NULL;
	ptranC = 0;
	tranC = 0;
}
glm::mat4 GeoObject::getTrans()
{
	//this was to have programing to check for if there is a new transformation 
	//was needed or not. but the tricky with the parent matrix I never got working, so it just
	//always updates the transformation matrix.
	updateTrans();
	return trans;
}
glm::vec3 GeoObject::getGlobalLoc()
{
	//this method returns an object in model space. regaurdless of camera or anything,
	//it just gets where the object is. because these are homogeneous coordinates, the
	//first 3 components of the vector get divided by the 4th to get consistency.
	glm::vec4 temp = getTrans() * glm::vec4(1.0f);
	return glm::vec3(temp.x/temp.w,temp.y/temp.w,temp.z/temp.w);
}
void GeoObject::move(float x, float y, float z)
{//this moves an object by the amount given. so this is an addative method. setLoc is not.
	if(!(x == 0 && y == 0 && z == 0))
	{
		loc = loc + glm::vec3(x,y,z);
		newTrans=false;
	}
}
void GeoObject::rotate(float _x, float _y, float _z)
{//this rotates an object by the amount given, and thus is addative, setRot is not.
	//these calculations are the same as below, follow link below for detail.
	if(!(_x == 0 && _y == 0 && _z == 0))
	{
		newTrans=false;

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

		//this is the important difference between rotate and setRot. this line
		//chains the rotations together allowing for an object to be rotated from 
		//it's current position at will in a logical way.
		rotquat = local * rotquat;
	}
}

void GeoObject::setLoc(glm::vec3 _loc)
{//this sets the location to the one passed. Regaurdless of what the current location.
	if(loc!=_loc)
	{
		loc = _loc;
		newTrans=false;
	}
}
void GeoObject::setRot(float _x, float _y, float _z)
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


	//this is a set of calculations i found here:
	//http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
	//it is a lot of maths, how exactly it works I'm not entirely positive. but it does.
	//let a sleeping dog lie!
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
void GeoObject::updateTrans()
{TRACE(3);
	//get the translation matrix.
	trans = glm::translate(glm::mat4(1.0f),loc);TRACE(5);
	//because of order of operations. the translation matrix is applied to the rotation
	//matrix not the other way around. so rotation is done first here. 
	trans = trans * glm::gtc::quaternion::mat4_cast(rotquat);TRACE(5);
	//lastly apply the parent transform.
	if (parent) trans = parent->getTrans() * trans;TRACE(5);
	newTrans = true;TRACE(5);
	++tranC;TRACE(3);
}
bool GeoObject::onEvent(const Event& event)
{
	TRACE(4);
	switch(event.type)
	{
		case EVENT_MOVE:
			move(event.args[0].datum.v_asFloat[0],event.args[0].datum.v_asFloat[1],event.args[0].datum.v_asFloat[2]);
			TRACE(3);
			return(true);
			break;
		case EVENT_ROTATE:
			rotate(event.args[0].datum.v_asFloat[0],event.args[0].datum.v_asFloat[1],event.args[0].datum.v_asFloat[2]);
			TRACE(3);
			return(true);
			break;
		case EVENT_SET_LOC:
			setLoc(glm::vec3(event.args[0].datum.v_asFloat[0],event.args[0].datum.v_asFloat[1],event.args[0].datum.v_asFloat[2]));
			return(true);
			break;
		case EVENT_SET_ROTATION:
			setRot(event.args[0].datum.v_asFloat[0],event.args[0].datum.v_asFloat[1],event.args[0].datum.v_asFloat[2]);
			return(true);
			break;
		default:
			return(false);
			break;
	}
}


/*.S.D.G.*/
#endif
