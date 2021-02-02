/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"

namespace rt{

class Sphere:public Shape{

public:

	//
	// Constructors
	//
	Sphere() {};
	Sphere(Vec3f center, float radius, Material* material);

	virtual ~Sphere();


	//
	// Functions that need to be implemented, since Sphere is a subclass of Shape
	//
	Hit intersect(Ray ray);

	//
	// gets the normal of the sphere at any point on the sphere
	//
	Vec3f getNormal(Vec3f p);

	//
	// print function (implementing abstract function of base class)
	//
	void printShape();

	//
	// gets the (u,v) mapping from a point on the sphere
	//
	Vec2f getTextureCoordinates(Vec3f p);


	std::vector<Vec3f> getBoundingBox();

private:

	Vec3f& centre = pos;
	float radius;

};



} //namespace rt




#endif /* SPHERE_H_ */
