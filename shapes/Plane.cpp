/*
 * Plane.cpp
 *
 *
 */
#include "shapes/Plane.h"
#include "core/RayTracer.h"


namespace rt {

	//
	//Plane constructors
	//
	Plane::Plane(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Material* newMaterial) {
		points[0] = a; points[1] = b; points[2] = c; points[3] = d;
		material = newMaterial;
		calculateNormal();
	}
	
	Plane::Plane(std::vector<Vec3f> newPoints, Material* newMaterial) {
		points = newPoints;
		material = newMaterial;
		calculateNormal();
	}

	//
	//Plane destructor
	//
	Plane::~Plane() {

	}

	Vec2f Plane::getTextureCoordinates(Vec3f p) {

		Vec3f bary;
		Vec3f A = points[0];
		Vec3f B = points[1];
		Vec3f C = points[2];
		float areaABC = normal.dotProduct((B - A).crossProduct(C - A));
		float areaPBC = normal.dotProduct((B - p).crossProduct(C - p));
		float areaPCA = normal.dotProduct((C - p).crossProduct(A - p));

		bary.x = areaPBC / areaABC;
		bary.y = areaPCA / areaABC;
		bary.z = 1 - bary.x - bary.y;

		Vec2f uv;
		uv.x = (bary.x * 0) + (bary.y * material->textureWidth) + (bary.z * material->textureWidth);
		uv.y = (bary.x * 0) + (bary.y * 0) + (bary.z * material->textureHeight);

		return uv;

	}

	float Plane::Wedge2D(Vec2f v, Vec2f w) {
		return v.x * w.y - v.y * w.x;
	}

	std::vector<Vec3f> Plane::getBoundingBox() {
		float maxx = points[0].x;
		float maxy = points[0].y;
		float maxz = points[0].z;

		float minx = points[0].x;
		float miny = points[0].y;
		float minz = points[0].z;

		for (int i = 0; i < points.size(); i++) {

			if (points[i].x > maxx) { maxx = points[i].x; }
			if (points[i].y > maxy) { maxy = points[i].y; }
			if (points[i].z > maxz) { maxz = points[i].z; }

			if (points[i].x < minx) { minx = points[i].x; }
			if (points[i].y < miny) { miny = points[i].y; }
			if (points[i].z < minz) { minz = points[i].z; }
		}

		Vec3f a = Vec3f(maxx, maxy, maxz);
		Vec3f b = Vec3f(minx, miny, minz);

		std::vector<Vec3f> ret;
		ret.push_back(a);
		ret.push_back(b);

		return ret;
	}


	//
	//Shape destructor
	//
	//Shape::~Shape() {}


	/**
	 * Computes whether a ray hit the specific instance of a triangle shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Plane::intersect(Ray ray) {

		Hit h;
		h.hit = false;
		h.shape = (Shape*)this;
		RayTracer::numIntersectChecks += 1;
		//-----------to be implemented -------------

		float denom = normal.dotProduct(ray.direction);
		h.normal = normal;

		if (std::abs(denom) > 1e-6) {
			Vec3f p0l0 = points[0] - ray.origin;
			float t = p0l0.dotProduct(normal) / denom;
			if (t >= 0) {
				h.point = ray.origin + (t * ray.direction);

				//Check that h point is within the plane
				Vec3f AM = h.point - points[0];
				Vec3f AB = points[1] - points[0];
				Vec3f AC = points[2] - points[0];
				Vec3f AD = points[3] - points[0];

				if ((0 < AM.dotProduct(AB)) && (AM.dotProduct(AB) < AB.dotProduct(AB))) {
					if ((0 < AM.dotProduct(AD)) && (AM.dotProduct(AD) < AD.dotProduct(AD))) {
						if ((0 < AM.dotProduct(AC)) && (AM.dotProduct(AC) < AC.dotProduct(AC))) {
							h.tmin = t;
							h.hit = true;
						}
					}
				}

				return h;
			}
		}

		return h;

	}

	void Plane::calculateDistanceFromOrigin() {
		
	}

	void Plane::calculateNormal() {
		Vec3f temp = points[0] - points[1];
		Vec3f temp1 = points[0] - points[2];
		normal = temp.crossProduct(temp1);
		normal = normal.normalize();
	}

	Vec3f Plane::getNormal(Vec3f p) {
		//return (p - centre) * (-1 / radius);
		return normal;
	}

	Vec3f Plane::getNormal() {
		return normal;
	}

	Vec3f Plane::getPos() {
		return points[0];
	}

	void Plane::setPos(Vec3f newPos) {
		this->points[0]= newPos;
	}

	std::vector<Vec3f> Plane::getPoints() {
		return points;
	}

	void Plane::setPoints(std::vector<Vec3f> newPoints) {
		if (newPoints.size() != 4) {
			return;
		}
		else {
			points = newPoints;
		}
	}


	/**
	* Prints plane data
	* this function implements the abstract function of the base Shape class
	*/
	void Plane::printShape() {
		printf("I am a plane! \n");
		printf("xpos: %f, ypos: %f, zpos: %f, radius: %f\n", points[0].x, points[0].y, points[0].z);
	}




} //namespace rt
