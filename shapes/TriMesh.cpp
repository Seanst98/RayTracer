/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"
#include "core/RayTracer.h"
#include "core/RayHitStructs.h"


namespace rt{

	//
	//Trimesh cosntructors
	//
	TriMesh::TriMesh(std::vector<Vec3f> vertices, Material* newmaterial, std::vector<Vec2f> textureCoords) {

		material = newmaterial;

		for (int i = 0; i < vertices.size()-2; i++) {

			Vec3f a, b, c;
			Vec2f at, bt, ct;
			std::vector<Vec2f> textCoordsV;

			/*if (i % 2 == 1) {

				a = vertices[i];
				b = vertices[i + 2];
				c = vertices[i + 1];

				at = textureCoords[i];
				bt = textureCoords[i + 2];
				ct = textureCoords[i + 1];


			}
			else {*/
				a = vertices[i];
				b = vertices[i + 1];
				c = vertices[i + 2];

				at = textureCoords[i];
				bt = textureCoords[i + 1];
				ct = textureCoords[i + 2];
			//}

			textCoordsV.push_back(at); textCoordsV.push_back(bt); textCoordsV.push_back(ct);

			textureCoordinates.push_back(textCoordsV);


			std::vector<Vec3f> points;
			points.push_back(a); points.push_back(b); points.push_back(c);

			Triangle* triangle = new Triangle(points, material);

			mesh.push_back(triangle);

		}
	}

	//
	//TriMesh deconstructor
	//
	TriMesh::~TriMesh() {}



	/**
	 * Computes whether a ray hit the mesh and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit TriMesh::intersect(Ray ray) {

		Hit h;
		h.hit = false;
		h.shape = (Shape*)this;
		//-----------to be implemented -------------

		Triangle* triangle = nullptr;
		float minDist = std::numeric_limits<float>::max();
		Vec3f nHit;
		Hit finalHit;
		finalHit.hit = false;

		//Loop over all triangles and check for intersections
		for (int i = 0; i < mesh.size(); i++) {
			h = mesh[i]->intersect(ray);

			if (h.hit == false) {
				continue;
			}

			nHit = mesh[i]->getNormal(h.point);
			nHit = nHit.normalize();

			Vec3f temp = (ray.origin - h.point) * (ray.origin - h.point);
			float temp2 = temp.x + temp.y + temp.z;
			float distance = sqrt(temp2);

			if (distance < minDist) {
				minDist = distance;
				triangle = mesh[i];
				finalHit = h;
				finalHit.tmin = h.tmin;
			}
		}

		if (!finalHit.hit) {
			return h;
		}

		finalHit.shape = (Shape*)this;
		return finalHit;

	}

	Vec2f TriMesh::getTextureCoordinates(Vec3f p) {
		
		//Find which triangle this point lies in
		for (int i = 0; i < mesh.size(); i++) {
			//edge 0
			Vec3f edge0 = mesh[i]->getPoints()[1] - mesh[i]->getPoints()[0];
			Vec3f vp0 = p - mesh[i]->getPoints()[0];
			Vec3f D = edge0.crossProduct(vp0);
			if (mesh[i]->getNormal().dotProduct(D) < 0) {
				continue;
			}

			//edge 1
			Vec3f edge1 = mesh[i]->getPoints()[2] - mesh[i]->getPoints()[1];
			Vec3f vp1 = p - mesh[i]->getPoints()[1];
			D = edge1.crossProduct(vp1);
			if (mesh[i]->getNormal().dotProduct(D) < 0) {
				continue;
			}

			//edge 2
			Vec3f edge2 = mesh[i]->getPoints()[0] - mesh[i]->getPoints()[2];
			Vec3f vp2 = p - mesh[i]->getPoints()[2];
			D = edge2.crossProduct(vp2);
			if (mesh[i]->getNormal().dotProduct(D) < 0) {
				continue;
			}

			Triangle* triangle = mesh[i];
			
			//So if we know the triangle the hit point lies on
			//And we know the triangles coords and it's vertices texture coordinates
			//We can interpolate between the texture coordinates to find the actual texture coordinates

			Vec2f uv;
			
			//find texture coords belonging to this triangle
			std::vector<Vec2f> textCoords = textureCoordinates[i];

			Vec2f P = triangle->getTextureCoordinates(p);
			P.x = P.x / material->textureWidth;
			P.y = P.y / material->textureHeight;


			Vec3f bary;
			Vec3f A = Vec3f(textCoords[0].x, textCoords[0].y, 0);
			Vec3f B = Vec3f(textCoords[1].x, textCoords[1].y, 0);
			Vec3f C = Vec3f(textCoords[2].x, textCoords[2].y, 0);

			float areaABC = triangle->getNormal().dotProduct((B - A).crossProduct(C - A));
			float areaPBC = triangle->getNormal().dotProduct((B - p).crossProduct(C - p));
			float areaPCA = triangle->getNormal().dotProduct((C - p).crossProduct(A - p));

			bary.x = areaABC == 0 ? 0.333333 : areaPBC / areaABC;
			bary.y = areaABC == 0 ? 0.333333 : areaPCA / areaABC;
			bary.z = 1 - bary.x - bary.y;


			float aas = bary.x + bary.y + bary.z;

			if (aas >= 1) {
				bary.x = bary.x * 0.999999;
				bary.y = bary.y * 0.999999;
				bary.z = bary.z * 0.999999;
			}
			uv.x = (bary.x * A.x) + (bary.y * B.x) + (bary.z * C.x);
			uv.y = (bary.x * A.y) + (bary.y * B.y) + (bary.z * C.y);

			do {

				if (uv.x < 0) {
					uv.x = 1 + uv.x;
				}
				if (uv.y < 0) {
					uv.y = 1 + uv.y;
				}
				if (uv.x > 1) {
					uv.x = uv.x - 1;
				}
				if (uv.y > 1) {
					uv.y = uv.y - 1;
				}

			} while (uv.x < 0 || uv.y < 0 || uv.x > 1 || uv.y > 1);

			return Vec2f(uv.x*material->textureWidth, uv.y*material->textureHeight);
		}

		//This shouldn't happen
		return Vec2f(0, 0);


	}

	std::vector<Vec3f> TriMesh::getBoundingBox() {

		std::vector<Vec3f> points;
		for (int i = 0; i < mesh.size(); i++) {
			std::vector<Vec3f> a = mesh[i]->getPoints();

			points.push_back(a[0]);
			points.push_back(a[1]);
			points.push_back(a[2]);
		}


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


	Vec3f TriMesh::getNormal(Vec3f p) {
		std::cout << "shouldn't be doing this naughty naughty" << std::endl;
		return Vec3f(0, 0, 0);
	}

	void TriMesh::printShape() {
		std::cout << "Yeah man this a tri mesh" << std::endl;
		std::cout << "PS if anyone reads this... This was a hard af CW" << std::endl;
	}

} //namespace rt

