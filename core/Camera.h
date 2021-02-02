/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "rapidjson/document.h"
#include "math/geometry.h"


using namespace rapidjson;

namespace rt{

class Camera{
public:

	//
	// Constructors
	//
	Camera(){};
	Camera(int height, int width, int fov, Vec3f pos, Vec3f lookAt, std::string newtype):height(height), width(width), fov(fov), pos(pos), lookAt(lookAt), type(newtype){};

	//
	// Destructor
	//
	virtual ~Camera();


	//
	// factory function : returns camera instance dynamically based on camera specifications
	//
	static Camera* createCamera(Value& cameraSpecs);


	//
	// print function (to be implemented by the subclasses )
	//
	virtual void printCamera()=0;



	//
	// other camera functions (to complete)
	//














	//
	// Getters and Setters
	//
	int getHeight() const {
		return height;
	}

	void setHeight(int height) {
		this->height = height;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	Vec3f getPos() {
		return pos;
	}

	void setPos(Vec3f newPos) {
		this->pos = newPos;
	}

	Vec3f getLookAt() {
		return lookAt;
	}

	void setLookAt(Vec3f newLookAt) {
		this->lookAt = newLookAt;
	}

	int getFov() {
		return fov;
	}

	void setFov(int newFov) {
		fov = newFov;
	}

	std::string getType() {
		return type;
	}

	void setType(std::string newtype) {
		type = newtype;
	}



protected:

	//
	//camera members
	//
	int height;
	int width;
	int fov; //field of view
	Vec3f pos;  //World position of camera
	Vec3f lookAt;  //World position camera looks at
	Matrix44f cameraToWorld;
	std::string type;

};

} //namespace rt



#endif /* CAMERA_H_ */
