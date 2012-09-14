/* this is class for holding the information concerning a camera:
 * Field of vision
 * Size of display
 * Clip near and far
 * and the combination of these for a quick and ready projection matrix
 * Location vector
 */

//Currently the Camera deals with a lot of the transform logic. I may be removing this down the
//road if I move the cameras as a concept up into openCL. I may still be able to use glm there, but
//for this application, I think I could get more effecient code if I hand wrote it to optimize the
//particulars of the situation. So for now, here are the gl math libraries:
#include "Camera.h"

//include gaurd.
#ifndef CAMERA_CPP
#define CAMERA_CPP
//the constructor. You must name:
//	field of vision: a half angle on the y. because the aspect ratio is known, the fov
//		for the x will be solved for. 
//	x,y: the size of the window. x is width, y is height. example: 800x600, 1920x1080
//	clipping near and far: these are the clipping planes. anything closer than near
//		will not get rendered, and anything further than far is out of sight.
//	loc: the initial location of the camera. 
Camera::Camera(int _fov, int _x, int _y, float _near, float _far)
{
	fov = _fov;
	x = _x;
	y = _y;
	near = _near;
	far = _far;
	newProj = 0;
}
Camera::Camera(xml_node self)
{//the constructor method for constructing form a the xml. most of these are self explanitory.
	fov = self.attribute("fov").as_float();
	x = self.attribute("dimX").as_float();
	y = self.attribute("dimY").as_float();
	near = self.attribute("clipNear").as_float();
	far = self.attribute("clipFar").as_float();
	newProj = 0;
	move(self.attribute("locX").as_float(),
		self.attribute("locY").as_float(),
		self.attribute("locZ").as_float());
	ID = globalRM->RequestID();
	globalRM->AssignID(ID,this);
	if(self.attribute("LTI"))
	{//register the load time identifier to the resource manager.
		TRACE(3);
		globalRM->RegisterLTI(self.attribute("LTI").value(),ID);
	}
	if(self.attribute("focus"))
	{//set the focus of the camera to some other object. this is done with an LTI.
		TRACE(3);
		Event e;
		e.args[0].datum.v_asInt[0] = (int)20;
		e.receiver = ID;
		e.type = EVENT_DELAYED_REQUEST;
		strcpy(e.args[2].datum.v_asChar,self.attribute("focus").value());
		globalRM->RegisterRequest(e);
	}
}
//destructor. is as you would expect.
Camera::~Camera()
{
	delete &projection;
	fov = 0;
	x = 0;
	y = 0;
	near = 0.0;
	far = 0.0;
	newProj = 0;
}
glm::mat4 Camera::lookAt(glm::vec3 val)
{//wrapper method for glm::lookAt function. this way the camera can be used to wrap this up
//neatly. you just have to tell the camera to look at something and it will manage the rest
	TRACE(3);
	if(parent)
	{
		TRACE(3);
		return glm::lookAt(getGlobalLoc(),val,glm::vec3(0.0f,1.0f,0.0f));
	}
	TRACE(3);
	return glm::lookAt(getGlobalLoc(),val,glm::vec3(0.0f,1.0f,0.0f));
}
glm::mat4 Camera::view()
{
	TRACE(3);
	if(focus) 
	{
		TRACE(3);
		return lookAt(focus->getGlobalLoc());
	}
	TRACE(3);
	return lookAt(glm::vec3(1.0f));
}
glm::mat4 Camera::model()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, loc.z));
}
glm::mat4 Camera::newProjection()
{//this method is used to create the new projection matrix. is calls the appropriate glm
//	function and sends to it the local data values it maintains.
	projection = glm::perspective(fov, 1.0f*x/y, near, far);
	newProj = 1;
	return projection;
}

bool Camera::onEvent(const Event& event)
{
	TRACE(3);
	if(event.type == EVENT_DELAYED_REQUEST)
	{
		TRACE(2);
		if(event.args[0].datum.v_asInt[0] == 20)
		{
			TRACE(3);
			string s(event.args[2].datum.v_asChar);
			TRACE(5);
			GeoObject* temp = (GeoObject*)(globalRM->GetIDRetaining(globalRM->ResolveLTI(s)));
			TRACE(5);
			setFocus(temp);
			TRACE(5);
			return true;
		}
		TRACE(3);
	}
	TRACE(5);
	return GeoObject::onEvent(event);
}

#endif
/*.S.D.G.*/
