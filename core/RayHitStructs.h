/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"
#include "Shape.h"

namespace rt{

/*
 * Ray structure definition
 */
enum RayType {PRIMARY, SECONDARY, SHADOW};

struct Ray{

	RayType raytype;

	//----------Ray variables to be filled------
	Vec3f origin;
	Vec3f direction;
};


struct Hit{

	Vec3f point; //point where ray hits a shape
	//----------Hit variables to be filled------
	bool hit;
	Vec3f normal;
	float tmin;
	Shape* shape;

};

}


#endif /* CORE_RAYHITSTRUCTS_H_ */
