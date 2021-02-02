/*
 * Triangle.cpp
 *
 *
 */
#include "Triangle.h"
#include "core/RayHitStructs.h"
#include "core/RayTracer.h"



namespace rt{

	//
	//Triangle constructors
	//
	Triangle::Triangle(Vec3f a, Vec3f b, Vec3f c, Material* newMaterial) {
		points[0] = a; points[1] = b; points[2] = c;
		material = newMaterial;
		calculateNormal();
	}

	Triangle::Triangle(std::vector<Vec3f> newPoints, Material* newMaterial) {
		points = newPoints;
		material = newMaterial;
		calculateNormal();
	}

	//
	//Plane destructor
	//
	Triangle::~Triangle() {

	}

	Vec2f Triangle::getTextureCoordinates(Vec3f p) {
		Vec3f A = points[0];
		Vec3f B = points[1];
		Vec3f C = points[2];
		float BaryATemp = ((B.y - C.y)*(p.x - C.x)) + ((C.x - B.x)*(p.y - C.y));
		float BaryATemp1 = ((B.y - C.y) * (A.x - C.x)) + ((C.x - B.x) * (A.y - C.y));

		float BaryA = BaryATemp / BaryATemp1;

		float BaryBTemp = ((C.y - A.y) * (p.x - C.x)) + ((A.x - C.x) * (p.y - C.y));
		float BaryBTemp1 = ((B.y - C.y) * (A.x - C.x)) + ((C.x - B.x) * (A.y - C.y));

		float BaryB = BaryBTemp / BaryBTemp1;

		float BaryC = 1 - BaryA - BaryB;

		Vec3f uv;
		A.x = A.x * BaryA; A.y = A.y * BaryA;
		B.x = B.x * BaryB; B.y = B.y * BaryB;
		C.x = C.x * BaryC; C.y = C.y * BaryC;

		float aas = BaryA + BaryB + BaryC;

		if (aas >= 1) {
			BaryC = BaryC * 0.999999;
			BaryB = BaryB * 0.999999;
		}
		uv.x = (BaryA * 0) + (BaryB * 0) + (BaryC * material->textureWidth);
		uv.y = (BaryA * 0) + (BaryB * material->textureHeight) + (BaryC * material->textureHeight);

		return Vec2f(uv.x, uv.y);
	}


	/**
	 * Computes whether a ray hit the specific instance of a triangle shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Triangle::intersect(Ray ray) {

		Hit h;
		h.hit = false;
		h.shape = (Shape*)this;
		RayTracer::numIntersectChecks += 1;
		//-----------to be implemented -------------


		if (std::abs(normal.dotProduct(ray.direction)) < 1e-6) {
			return h;
		}

		float d = normal.dotProduct(points[0]);

		float t = (d - normal.dotProduct(ray.origin)) / (normal.dotProduct(ray.direction));

		if (t < 0) {
			return h;
		}

		h.point = ray.origin + t * ray.direction;

		//edge 0
		Vec3f edge0 = points[1] - points[0];
		Vec3f vp0 = h.point - points[0];
		Vec3f C = edge0.crossProduct(vp0);
		if (normal.dotProduct(C) < 0) {
			return h;
		}

		//edge 1
		Vec3f edge1 = points[2] - points[1];
		Vec3f vp1 = h.point - points[1];
		C = edge1.crossProduct(vp1);
		if (normal.dotProduct(C) < 0) {
			return h;
		}

		//edge 2
		Vec3f edge2 = points[0] - points[2];
		Vec3f vp2 = h.point - points[2];
		C = edge2.crossProduct(vp2);
		if (normal.dotProduct(C) < 0) {
			return h;
		}

		h.hit = true;
		h.normal = getNormal();
		h.tmin = t;
		return h;

	}

	std::vector<Vec3f> Triangle::getBoundingBox() {
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


	void Triangle::calculateNormal() {
		Vec3f temp = points[0] - points[1];
		Vec3f temp1 = points[0] - points[2];
		normal = temp.crossProduct(temp1);
		normal = normal.normalize();
	}

	Vec3f Triangle::getNormal(Vec3f p) {
		//return (p - centre) * (-1 / radius);
		return normal;
	}

	void Triangle::setNormal(Vec3f newnormal) {
		normal = -newnormal.normalize();
		std::cout << "sdjf";
	}

	Vec3f Triangle::getNormal() {
		return normal;
	}

	Vec3f Triangle::getPos() {
		return points[0];
	}

	void Triangle::setPos(Vec3f newPos) {
		this->points[0] = newPos;
	}

	std::vector<Vec3f> Triangle::getPoints() {
		return points;
	}

	void Triangle::setPoints(std::vector<Vec3f> newPoints) {
		if (newPoints.size() != 4) {
			return;
		}
		else {
			points = newPoints;
		}
	}

	/**
	* Prints triangle data
	* this function implements the abstract function of the base Shape class
	*/
	void Triangle::printShape() {
		printf("I am a plane! \n");
		printf("xpos: %f, ypos: %f, zpos: %f, radius: %f\n", points[0].x, points[0].y, points[0].z);
	}

} //namespace rt

