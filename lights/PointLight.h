/*
 * PointLight.h
 *
 *
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "core/LightSource.h"


namespace rt{

class PointLight: public LightSource{
public:

	//
	//Constructors
	//
	PointLight();
	PointLight(Vec3f pos, Vec3f intensity, std::string type);

	//
	//Destructor
	//
	~PointLight() {};

	//
	// print function (implementing abstract function of base class)
	//
	void printLight();


};



} //namespace rt




#endif /* POINTLIGHT_H_ */