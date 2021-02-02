/*
 * BlinnPhong.h
 *
 *
 */

#ifndef BLINNPHONG_H_
#define BLINNPHONG_H_

#include "core/Material.h"
#include "core/Shape.h"
#include "core/LightSource.h"

namespace rt{

class BlinnPhong: public Material{

public:
	//
	//Constructors
	//
	BlinnPhong() {};

	Vec3f calculateShade(Vec3f nHit, Vec3f hitPoint, Vec3f shadowDirection, Vec3f rayDirection, Shape* shape, Vec3f lightIntensity);

};



} //namespace rt



#endif /* BLINNPHONG_H_ */
