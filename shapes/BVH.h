/*
 * BVH.h
 *
 *
 */

#ifndef BVH_H_
#define BVH_H_

#include "core/Shape.h"
#include <vector>
#include "shapes/TriMesh.h"
#include <stack>

namespace rt{

class BVH: public Shape{

	class BVHNode {

	public:
		void setBoundingBox(Vec3f newvmax, Vec3f newvmin) {
			vmax = newvmax;
			vmin = newvmin;
		}
		//void makeLeaf(int index, int nShapes);
		void makeLeaf() { leaf = true; }
		void makeNode(int leftIndex, int nShapes);

		Vec3f vmin;
		Vec3f vmax;
		bool leaf;
		int nShapes;
		int index;
		std::vector<Shape*> shapeList;
		std::vector<Vec3f> centroids;
		BVHNode* bvh;
		BVHNode* bvh1;

		std::vector<Vec3f> getBoundingBox() {
			std::vector<Vec3f> a;
			a.push_back(vmax); a.push_back(vmin);
			return a;
		};

		Hit intersect(Ray ray);



	private:

	};

	struct StackItem {
		BVHNode* node;
		float t;
	};

public:


	//
	//Constructors
	//
	BVH();
	BVH(std::vector<Shape*> shapeList);


	void recursiveBuild(int left_index, int right_index, BVHNode* node, int depth);

	~BVH() {};

	Hit intersect(Ray ray);

	Vec3f getNormal(Vec3f p);

	Vec3f getPos();
	void setPos(Vec3f);

	void printShape();

	Vec3f getFarthest(std::vector<Vec3f> points, Vec3f d);

	std::vector<Vec3f> getBoundingBox(std::vector<Vec3f> points);
	std::vector<Vec3f> getBoundingBox();

	Vec2f getTextureCoordinates(Vec3f p);

	float calculateDistance(Vec3f a, Vec3f b);



private:
	BVHNode* rootNode;
	Vec3f vmin;
	Vec3f vmax;
	std::vector<BVHNode*> nodeList;
	std::stack<StackItem> stack;  //Stack used for keeping track of bounding volumes hit
};



} //namespace rt



#endif /* BVH_H_ */
