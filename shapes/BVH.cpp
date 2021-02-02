/*
 * BVH.cpp
 *
 *
 */
#include "BVH.h"
#include <algorithm>
#include <queue>
#include "core/RayTracer.h"
#include "core/RayHitStructs.h"


namespace rt{
	//
	//BVH constructor
	//
	BVH::BVH(std::vector<Shape*> shapes) {

		//To build the heriarchy we must find the root bounding volume
		//Find the max x,y,z and min x,y,z of the bounding volume

		int left_index = 0, right_index = shapes.size(), numNodes = 1;
		BVHNode* rootNodex = new BVHNode();  //root node

		//Get a list of points which describe volume boxes of each shape
		std::vector<Vec3f> points;
		std::vector<Vec3f> centroids;
		for (int i = 0; i < shapes.size(); i++) {
			std::vector<Vec3f> maxMinShape = shapes[i]->getBoundingBox();
			points.push_back(maxMinShape[0]);
			points.push_back(maxMinShape[1]);
			Vec3f centroid = maxMinShape[0] + maxMinShape[1];
			centroid.x = centroid.x / 2; centroid.y = centroid.y / 2; centroid.z = centroid.z / 2;
			centroids.push_back(centroid);

		}


		//Find root node volume box
		std::vector<Vec3f> bounds = getBoundingBox(points);

		vmax = bounds[0];
		vmin = bounds[1];

		rootNodex->setBoundingBox(vmax, vmin);
		rootNodex->shapeList = shapes;
		rootNodex->centroids = centroids;
		nodeList.push_back(rootNodex);
		rootNode = rootNodex;

		recursiveBuild(0, shapes.size(), rootNode, 0);
		std::cout << "BVH BUILT" << std::endl;

	}

	void BVH::recursiveBuild(int left_index, int right_index, BVHNode* node, int depth) {

		std::cout << depth << std::endl;

		if (node->shapeList.size()==1) {
			//Initiate current node as a leaf with primitives from shapes[left_index] to shapes[right_index]
			node->leaf = true;
			return;
		}
		else {
			//We want to recursively split this bounding box until there is only
			//A shape at the bottom

			//Split via mid point on the largest axis

			std::vector<Shape*> leftShapes;
			std::vector<Vec3f> leftCentroids;
			std::vector<Shape*> rightShapes;
			std::vector<Vec3f> rightCentroids;

			Vec3f largestAxis = (node->vmax - node->vmin);
			largestAxis.x = std::abs(largestAxis.x);
			largestAxis.y = std::abs(largestAxis.y);
			largestAxis.z = std::abs(largestAxis.z);


			//Then sort the bounding box centroids in the largest axis direction
			//And split into a left and right side

			do {

				if (largestAxis.x > largestAxis.y && largestAxis.x > largestAxis.z) {
					//Split on the x axis
					//float mid = (node->vmax.x + node->vmin.x) / 2;
					float mid = (largestAxis.x + (2 * node->vmin.x)) / 2;

					//Check all centroids against this midpoint
					//And split to right or left. Must take care to split shapes too
					//So when we recurse it's the correct shapes
					for (int i = 0; i < node->centroids.size(); i++) {

						if (node->centroids[i].x <= mid) {
							//Split to left
							leftShapes.push_back(node->shapeList[i]);
							leftCentroids.push_back(node->centroids[i]);
						}
						else {
							//Split to right
							rightShapes.push_back(node->shapeList[i]);
							rightCentroids.push_back(node->centroids[i]);
						}
					}

					if (leftShapes.size() == 0 || rightShapes.size() == 0) {
						//we gotta try another axis
						largestAxis.x = largestAxis.x/2;
						leftShapes.clear();
						leftCentroids.clear();
						rightShapes.clear();
						rightCentroids.clear();
					}
					else {
						break;
					}

				}
				else if (largestAxis.y > largestAxis.x && largestAxis.y > largestAxis.z) {
					//Split on the y axis
					//float mid = (node->vmax.y + node->vmin.y) / 2;
					float mid = (largestAxis.y + (2 * node->vmin.y)) / 2;

					//Check all centroids against this midpoint
					//And split to right or left. Must take care to split shapes too
					//So when we recurse it's the correct shapes
					for (int i = 0; i < node->centroids.size(); i++) {

						if (node->centroids[i].y <= mid) {
							//Split to left
							leftShapes.push_back(node->shapeList[i]);
							leftCentroids.push_back(node->centroids[i]);
						}
						else {
							//Split to right
							rightShapes.push_back(node->shapeList[i]);
							rightCentroids.push_back(node->centroids[i]);
						}
					}

					if (leftShapes.size() == 0 || rightShapes.size() == 0) {
						//we gotta try another axis
						largestAxis.y = largestAxis.y/2;
						leftShapes.clear();
						leftCentroids.clear();
						rightShapes.clear();
						rightCentroids.clear();
					}
					else {
						break;
					}
				}
				else {
					//Split on the z axis
					//float mid = (node->vmax.z + node->vmin.z) / 2;
					float mid = (largestAxis.z + (2 * node->vmin.z)) / 2;

					//Check all centroids against this midpoint
					//And split to right or left. Must take care to split shapes too
					//So when we recurse it's the correct shapes
					for (int i = 0; i < node->centroids.size(); i++) {

						if (node->centroids[i].z <= mid) {
							//Split to left
							leftShapes.push_back(node->shapeList[i]);
							leftCentroids.push_back(node->centroids[i]);
						}
						else {
							//Split to right
							rightShapes.push_back(node->shapeList[i]);
							rightCentroids.push_back(node->centroids[i]);
						}
					}

					if (leftShapes.size() == 0 || rightShapes.size() == 0) {
						//we gotta try another axis
						largestAxis.z = largestAxis.z / 2;
						leftShapes.clear();
						leftCentroids.clear();
						rightShapes.clear();
						rightCentroids.clear();
					}
					else {
						break;
					}

				}

			} while (leftShapes.size() == 0 || rightShapes.size() == 0);


			//Calculate bounding box of the left and right sides
			std::vector<Vec3f> Lpoints;
			for (int i = 0; i < leftShapes.size(); i++) {
				std::vector<Vec3f> maxMinShape = leftShapes[i]->getBoundingBox();
				Lpoints.push_back(maxMinShape[0]);
				Lpoints.push_back(maxMinShape[1]);
			}

			std::vector<Vec3f> LBound = getBoundingBox(Lpoints);

			std::vector<Vec3f> Rpoints;
			for (int i = 0; i < rightShapes.size(); i++) {
				std::vector<Vec3f> maxMinShape = rightShapes[i]->getBoundingBox();
				Rpoints.push_back(maxMinShape[0]);
				Rpoints.push_back(maxMinShape[1]);
			}

			std::vector<Vec3f> RBound = getBoundingBox(Rpoints);


			BVHNode* leftNode = new BVHNode();
			BVHNode* rightNode = new BVHNode();

			leftNode->vmax = LBound[0];
			leftNode->vmin = LBound[1];
			leftNode->shapeList = leftShapes;
			leftNode->centroids = leftCentroids;
			rightNode->vmax = RBound[0];
			rightNode->vmin = RBound[1];
			rightNode->shapeList = rightShapes;
			rightNode->centroids = rightCentroids;

			nodeList.push_back(leftNode);
			nodeList.push_back(rightNode);

			node->bvh = leftNode;
			node->bvh1 = rightNode;

			recursiveBuild(left_index, leftShapes.size(), leftNode, depth + 1);
			recursiveBuild(leftShapes.size(), right_index, rightNode, depth + 1);

		}
	}

	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit BVH::intersect(Ray ray) {

		Hit h;
		h.hit = false;

		while (!stack.empty()) {
			stack.pop();
		}
		//-----------to be implemented -------------

		//Check the world bounding box for a intersection

		h = rootNode->intersect(ray);

		//If we don't hit the world bounding box
		if (h.hit == false) {
			return h;
		}

		h.hit = false;
		h.tmin = std::numeric_limits<float>::max();

		BVHNode* currentNode = rootNode;

		while (1) {
			if (currentNode->leaf) {
				//For each shape in the bounding box perform intersection testing
				Shape* shape = nullptr;
				float minDist = std::numeric_limits<float>::max();
				Vec3f nHit;  //Normal to the point of intersection
				Hit shapeHit;
				Hit finalHit;
				finalHit.hit = false;

				for (int i = 0; i < currentNode->shapeList.size(); i++) {

					shapeHit = currentNode->shapeList[i]->intersect(ray);
					shapeHit.shape = currentNode->shapeList[i];

					nHit = shapeHit.normal;
					nHit = nHit.normalize();

					Vec3f temp = (ray.origin - shapeHit.point) * (ray.origin - shapeHit.point);
					float temp2 = temp.x + temp.y + temp.z;
					float distance = sqrt(temp2);

					if (!shapeHit.hit) {
						continue;
					}

					if (distance < minDist) {
						minDist = distance;
						finalHit = shapeHit;
						h = finalHit;
						h.tmin = distance;
					}
				}

			}
			else {
				//Check intersection with children nodes
				Hit h1 = currentNode->bvh->intersect(ray);
				Hit h2 = currentNode->bvh1->intersect(ray);

				//If both boxes are hit then make currentNode the closest one and
				//Put the furthest away one on the stack
				h1.tmin = calculateDistance(ray.origin, h1.point);
				h2.tmin = calculateDistance(ray.origin, h2.point);

				if (h1.hit == false) {
					h1.tmin = std::numeric_limits<float>::max();
				}
				if (h2.hit == false) {
					h2.tmin = std::numeric_limits<float>::max();
				}
				if (h1.hit == true && h2.hit == true) {

					if (h1.tmin < h2.tmin){
						StackItem item; item.node = currentNode->bvh1; item.t = h2.tmin;
						stack.push(item);
						currentNode = currentNode->bvh;
					}
					else {
						StackItem item; item.node = currentNode->bvh; item.t = h1.tmin;
						stack.push(item);
						currentNode = currentNode->bvh1;
					}
					continue;
				}
				//If one is hit then set currentNode to the hit one
				else if (h1.hit == true || h2.hit == true) {
					currentNode = h1.hit ? currentNode->bvh : currentNode->bvh1;
					continue;
				}

				//If no box is hit then do nothing and let the stack popping code execute

			}

			//Pop the stack until we find one with a closer t
			if (stack.empty()) {
				return h;
			}

			while (!stack.empty()) {
				//If found a node that is closer than the current
				if (stack.top().t < h.tmin) {
					currentNode = stack.top().node;
					stack.pop();
					break;

				}
				else {
					//Pop the stack because these bounding boxes are further away
					//Than what we've already hit
					stack.pop();
				}
			}


		}

		h.hit = false;
		return h;

	}

	//Computes whether a ray has intersected with a nodes bounding box
	Hit BVH::BVHNode::intersect(Ray ray) {

		RayTracer::numIntersectChecks += 1;
		Hit h;
		h.hit = false;

		float tmin = 0;
		float tmax = 0;

		Vec3f invdir = 1 / ray.direction;

		if (invdir.x >= 0) {
			tmin = (vmin.x - ray.origin.x) * invdir.x;
			tmax = (vmax.x - ray.origin.x) * invdir.x;
		}
		else {
			tmin = (vmax.x - ray.origin.x) * invdir.x;
			tmax = (vmin.x - ray.origin.x) * invdir.x;
		}

		float tymin = 0;
		float tymax = 0;

		if (invdir.y >= 0) {
			tymin = (vmin.y - ray.origin.y) * invdir.y;
			tymax = (vmax.y - ray.origin.y) * invdir.y;
		}
		else {
			tymin = (vmax.y - ray.origin.y) * invdir.y;
			tymax = (vmin.y - ray.origin.y) * invdir.y;
		}

		float tzmin = 0;
		float tzmax = 0;

		if (invdir.z >= 0) {
			tzmin = (vmin.z - ray.origin.z) * invdir.z;
			tzmax = (vmax.z - ray.origin.z) * invdir.z;
		}
		else {
			tzmin = (vmax.z - ray.origin.z) * invdir.z;
			tzmax = (vmin.z - ray.origin.z) * invdir.z;
		}


		if (tymin > tymax) {
			std::swap(tymin, tymax);
		}

		if ((tmin > tymax) || (tymin > tmax)) {
			return h;
		}

		if (tymin > tmin) {
			tmin = tymin;
		}

		if (tymax < tmax) {
			tmax = tymax;
		}


		if (tzmin > tzmax) {
			std::swap(tzmin, tmax);
		}

		if ((tmin > tzmax) || (tzmin > tmax)) {
			return h;
		}

		if (tzmin > tmin) {
			tmin = tzmin;
		}

		if (tzmax < tmax) {
			tmax = tzmax;
		}


		if (tmin > tmax) std::swap(tmin, tmax);

		if (tmin < 0) {
			tmin = tmax;  //If t0 is negative, use t1 instead
			if (tmin < 0) {  //Both t0 and t1 are negative
				return h;
			}
		}

		h.hit = true;
		h.tmin = tmin;
		h.point = ray.origin + (ray.direction * h.tmin);

		return h;

	}

	float BVH::calculateDistance(Vec3f a, Vec3f b) {
		Vec3f temp = (a - b) * (a - b);
		float temp2 = temp.x + temp.y + temp.z;
		float distance = sqrt(temp2);

		return distance;
	}

	//Get the farthest point in a certain direction
	Vec3f BVH::getFarthest(std::vector<Vec3f> points, Vec3f direction) {

		Vec3f farthest;
		float farDistance = std::numeric_limits<float>::min();

		for (int i = 0; i < points.size(); i++) {

			float dot = points[i].dotProduct(direction);

			if (dot > farDistance) {
				farDistance = dot;
				farthest = points[i];
			}
		}

		return farthest;

	}

	std::vector<Vec3f> BVH::getBoundingBox(std::vector<Vec3f> points) {
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

		std::vector<Vec3f> ret; ret.push_back(a); ret.push_back(b);

		return ret;
	}

	std::vector<Vec3f> BVH::getBoundingBox() {
		std::vector<Vec3f> ret;
		ret.push_back(vmax);
		ret.push_back(vmin);

		return ret;
	}

	Vec3f BVH::getNormal(Vec3f p) {
		return Vec3f(0, 0, 0);
	}

	void BVH::printShape() {
		std::cout << "I am a bounding box" << std::endl;
	}

	Vec3f BVH::getPos() {
		return vmax;
	}

	void BVH::setPos(Vec3f) {
		//do nothin i dont wanna use this func ever
	}

	Vec2f BVH::getTextureCoordinates(Vec3f p) {
		return Vec2f(0, 0);
	}



} //namespace rt