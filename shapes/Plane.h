/*
 * Plane.h
 *
 *
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include <vector>

namespace rt {

	class Plane :public Shape {

	public:

		//
		// Constructors
		//
		Plane() {};
		Plane(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Material* material);
		Plane(std::vector<Vec3f>, Material* material);

		virtual ~Plane();


		//
		// Functions that need to be implemented, since Plane is a subclass of Shape
		//
		Hit intersect(Ray ray);

		//
		// gets the normal of the plane at any point on the plane
		//
		Vec3f getNormal(Vec3f p);
		Vec3f getNormal();

		//Overriding shape functions for use in plane
		Vec3f getPos();
		void setPos(Vec3f p);

		Vec2f getTextureCoordinates(Vec3f p);
		float Wedge2D(Vec2f a, Vec2f b);
		std::vector<Vec3f> getBoundingBox();

		//Gets plane points and sets them
		std::vector<Vec3f> getPoints();
		void setPoints(std::vector<Vec3f>);

		//Calculates the planes normal, used in the constructors as the normal is constant
		void calculateNormal();
		void calculateDistanceFromOrigin();

		//
		// print function (implementing abstract function of base class)
		//
		void printShape();

	private:

		std::vector<Vec3f> points;
		Vec3f normal;
		float d;  //distance from origin
		

	};



} //namespace rt




#endif /* PLANE_H_ */
