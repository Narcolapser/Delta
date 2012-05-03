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
#include "ResourceManager.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GeoObject.h"

//include gaurd.
#ifndef CAMERA_H
#define CAMERA_H
class Camera: public GeoObject
{
public:
	//the constructor. You must name:
	//	field of vision: a half angle on the y. because the aspect ratio is known, the fov
	//		for the x will be solved for. 
	//	x,y: the size of the window. x is width, y is height. example: 800x600, 1920x1080
	//	clipping near and far: these are the clipping planes. anything closer than near
	//		will not get rendered, and anything further than far is out of sight.
	//	loc: the initial location of the camera. 
	Camera(int _fov, int _x, int _y, float _near, float _far);
	Camera(xml_node self);
	~Camera();
	glm::mat4 lookAt(glm::vec3 val);
	glm::mat4 view();
	glm::mat4 model();
	void setFocus(GeoObject* _focus) {focus = _focus;}
	glm::mat4 inline getProjection(){if(newProj) return projection; return newProjection();}
	//this is kind of a clever little function. It checks the sentinal value "newProj". if the
	//	current projection in projection is the most up to date projection, or if there 
	//	even is one to be had. it is so, then it returns that projection, but if not, it
	//	will call the method to create the newProjection matrix. This way this method 
	//	always will return the appropriate and most up to date projection matrix. but, 
	//	because the projection matrix only changes when the field of vision, clipping
	//	planes or perspective change, it reduces the overhead significantly.
	glm::vec3 inline getLoc() {return loc;}		//get the location matrix
	float inline getFov() {return fov;}		//get the field of vision
	int inline getScreenX() {return x;}		//get the screen size on x
	int inline getScreenY() {return y;}		//get the screen size on y
	float inline getFarClip() {return near;}	//get the far clipping distance
	float inline getNearClip() {return far;}	//get the near clipping distance.

	void inline setLoc(glm::vec3 val) { loc = val;}	//set the location of the camera.
	void inline setFov(float val) {fov = val; newProj = 0;}	//set the field of vision.
	void inline setScreen(int valX, int valY) {x = valX; y = valY; newProj = 0;}
		//change the size of window
	void inline setNearClip(float val) {near = val; newProj = 0;}	//set the near clipping plane
	void inline setFarClip(float val) {far = val; newProj = 0;}	//set the far clipping plane
	bool onEvent(const Event& event);
	
//private:
	glm::mat4 newProjection();
//	glm::vec3 loc;		//location vector.
	GeoObject *focus;	//something to look at
	glm::mat4 projection;	//store for the projection matrix.
	float fov;		//field of vision
	float near;		//near clipping plane.
	float far;		//far clipping plane.
	int x;			//width of window
	int y;			//height of window
	int newProj;		//is the projection matrix the most recent one? yes=1, no=0
};
#endif
/*.S.D.G.*/

