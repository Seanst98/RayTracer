/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"
#include "core/RayTracer.h"


namespace rt{

	//
	//Sphere constructor
	//
	Sphere::Sphere(Vec3f newCentre, float newRadius, Material* newMaterial) {
		centre = newCentre;
		radius = newRadius;
		material = newMaterial;
	}

	//
	//Sphere destructor
	//
	Sphere::~Sphere() {

	}

	//
	//Shape destructor
	//
	Shape::~Shape() {}


	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Sphere::intersect(Ray ray){

		Hit h;
		h.hit = false;
		h.shape = (Shape*)this;
		RayTracer::numIntersectChecks += 1;
		//-----------to be implemented -------------

		float t0, t1;

		Vec3f L = centre - ray.origin;
		float tca = L.dotProduct(ray.direction);
		float d2 = L.dotProduct(L) - tca * tca;

		if (d2 > (radius*radius)) {
			return h;
		}

		float thc = sqrt((radius*radius) - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1;  //If t0 is negative, use t1 instead
			if (t0 < 0) {  //Both t0 and t1 are negative
				return h;
			}
		}

		h.hit = true;

		//Find hit co-ordinate
		h.point = ray.origin + (t0 * ray.direction);
		h.normal = getNormal(h.point);
		h.tmin = t0;

		return h;

	}

	Vec3f Sphere::getNormal(Vec3f p) {
		//return (p - centre) * (-1 / radius);
		return (p - centre).normalize();
	}

	//Calculates the texture coordinates from a point on the sphere
	Vec2f Sphere::getTextureCoordinates(Vec3f p) {

		Vec3f d = p - centre;
		d = d.normalize();

		float u = 0.5 + (atan2(d.z, d.x) / (2 * 3.14159265358979323846264338327950288));

		float v = 0.5 - (asin(d.y) / 3.14159265358979323846264338327950288);
		Vec2f uv; uv.x = u*material->textureWidth; uv.y = v*material->textureHeight;

		return uv;

	}

	std::vector<Vec3f> Sphere::getBoundingBox() {
		Vec3f max = centre + radius;
		Vec3f min = centre - radius;

		std::vector<Vec3f> ret;
		ret.push_back(max);
		ret.push_back(min);

		return ret;
	}


	/**
	* Prints sphere data
	* this function implements the abstract function of the base Shape class
	*/
	void Sphere::printShape() {
		printf("I am a sphere! \n");
		printf("xpos: %f, ypos: %f, zpos: %f, radius: %f\n", centre.x, centre.y, centre.z, radius);
	}




} //namespace rt
