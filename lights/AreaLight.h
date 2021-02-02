/*
 * AreaLight.h
 *
 *
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "core/LightSource.h"

namespace rt{

class AreaLight:public LightSource{

public:
	//
	//Constructors
	//
	AreaLight();
	AreaLight(Vec3f pos, Vec3f intensity, std::string type, float radius);

	//
	//Destructor
	//
	~AreaLight() {};

	//
	// print function (implementing abstract function of base class)
	//
	void printLight();


	float radius;

};



} //namespace rt




#endif /* AREALIGHT_H_ */