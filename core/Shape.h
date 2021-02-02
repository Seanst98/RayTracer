/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "rapidjson/document.h"
//#include "core/RayHitStructs.h"
#include "core/Material.h"
#include "math/geometry.h"
#include <vector>

using namespace rapidjson;


namespace rt{

	class Hit;
	class Ray;

class Shape{
public:

	//
	// Constructors
	//
	Shape() {};
	Shape(Vec3f pos) :pos(pos) {};

	//
	// Destructor (must be overriden in subclass)
	//
	virtual ~Shape();

	//
	// factory function : returns shape instance dynamically based on shape specifications
	//
	static Shape* createShape(Value& shapeSpecs);

	//
	// print function (to be implemented by the subclasses )
	//
	virtual void printShape() = 0;

	//
	// Shape abstract methods (to be implemented by subclasses)
	//
	virtual Hit intersect(Ray)=0;

	//
	// get the normal of the shape
	//
	virtual Vec3f getNormal(Vec3f pos)=0;

	//
	// Retrieve the material from the JSON
	//
	static Material* retrieveMaterial(Value& shapeSpecs);

	//
	// Get the texture coordinates from a point on the shape, base classes must implement
	//
	virtual Vec2f getTextureCoordinates(Vec3f p)=0;

	//
	// Get the bounding box of the shape
	//
	// Returns the max x,y,z and min x,y,z
	virtual std::vector<Vec3f> getBoundingBox()=0;

	//
	// Getters and Setters
	//

	Vec3f getPos() {
		return pos;
	}

	void setPos(Vec3f newPos) {
		this->pos = newPos;
	}

	Material* getMaterial() {
		return material;
	}

	void setMaterial(Material* newMaterial) {
		this->material = newMaterial;
	}

protected:

	Material* material;
	Vec3f pos;  //World position of the shape
	Matrix44f worldTransform;

};


} //namespace rt


#endif /* SHAPE_H_ */

