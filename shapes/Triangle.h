/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/Shape.h"
#include <vector>

namespace rt{

class Triangle: public Shape{

public:

	//
	//Constructors
	//
	Triangle() {};
	Triangle(Vec3f a, Vec3f b, Vec3f c, Material* material);
	Triangle(std::vector<Vec3f>, Material* material);

	virtual ~Triangle();

	//
	// Functions that need to be implemented, since Triangle is a subclass of Shape
	//
	Hit intersect(Ray ray);

	//
	// gets the normal of the triangle at any point on the triangle
	//
	Vec3f getNormal(Vec3f p);
	Vec3f getNormal();

	void setNormal(Vec3f newnormal);

	//Overriding shape functions for use in triangle
	Vec3f getPos();
	void setPos(Vec3f p);

	Vec2f getTextureCoordinates(Vec3f p);

	//Gets triangle points and sets them
	std::vector<Vec3f> getPoints();
	void setPoints(std::vector<Vec3f>);

	std::vector<Vec3f> getBoundingBox();

	//Calculates the triangles normal, used in the constructors as the normal is constant
	void calculateNormal();

	//
	// print function (implementing abstract function of base class)
	//
	void printShape();

private:
	std::vector<Vec3f> points;
	Vec3f normal;
};



} //namespace rt




#endif /* TRIANGLE_H_ */
