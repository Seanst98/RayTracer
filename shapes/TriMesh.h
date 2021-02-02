/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"
#include "shapes/Triangle.h"
#include <vector>

namespace rt{

class TriMesh: public Shape{

public:

	//
	//Constructors
	//
	TriMesh() {};
	TriMesh(std::vector<Vec3f> vertices, Material* material, std::vector<Vec2f> textCoords);


	~TriMesh();

	//
	// Functions that need to be implemented, since TriMesh is a subclass of Shape
	//
	Hit intersect(Ray ray);

	//
	// gets the normal of the triangle at any point on the trimesh
	//
	Vec3f getNormal(Vec3f p);
	//Vec3f getNormal();


	//Overriding shape functions for use in trimesh
	Vec3f getPos() {};
	void setPos(Vec3f p) {};

	//
	// print function (implementing abstract function of base class)
	//
	void printShape();


	Vec2f getTextureCoordinates(Vec3f p);

	std::vector<Vec3f> getBoundingBox();

private:
	std::vector<Triangle*> mesh;
	std::vector<std::vector<Vec2f>> textureCoordinates;  //Vector of vector as we want to relate the texture coordinates to the specific triangle they belong to


};



} //namespace rt




#endif /* TRIMESH_H_ */