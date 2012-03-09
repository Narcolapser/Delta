/* this is class for holding the information concerning a camera:
 * Field of vision
 * Size of display
 * Clip near and far
 * and the combination of these for a quick and ready projection matrix
 * Location vector
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(int _fov, int _x, int _y, float _near, float _far, glm::vec3 _loc)
	{
		fov = _fov;
		x = _x;
		y = _y;
		near = _near;
		far = _far;
		loc = _loc;
		newProj = 0;
	}
	~Camera()
	{
		delete &projection;
		delete &loc;
		fov = 0;
		x = 0;
		y = 0;
		near = 0.0;
		far = 0.0;
		newProj = 0;
	}
	glm::mat4 lookAt(glm::vec3 val)
	{
		return glm::lookAt(loc,val,glm::vec3(0.0f,1.0f,0.0f));
	}

	glm::mat4 inline getProjection(){if(newProj) return projection; return newProjection();}
	glm::vec3 inline getLoc() {return loc;}
	float inline getFov() {return fov;}
	int inline getScreenX() {return x;}
	int inline getScreenY() {return y;}
	float inline getFarClip() {return near;}
	float inline getNearClip() {return far;}

	void inline setLoc(glm::vec3 val) { loc = val;}
	void inline setFov(float val) {fov = val; newProj = 0;}
	void inline setScreen(int valX, int valY) {x = valX; y = valY; newProj = 0;}
	void inline setNearClip(float val) {near = val; newProj = 0;}
	void inline setFarClip(float val) {far = val; newProj = 0;}
	
private:
	glm::mat4 newProjection()
	{
		newProj = 1;
		projection = glm::perspective(fov, 1.0f*x/y, near, far);
		return projection;
	}
	glm::vec3 loc;
	glm::mat4 projection;
	float fov;
	float near;
	float far;
	int x;
	int y;
	int newProj;
};
