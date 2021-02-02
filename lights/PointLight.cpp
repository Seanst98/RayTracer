/*
 * PointLight.cpp
 *
 *
 */
#include "PointLight.h"



namespace rt{

	//
	//PointLight constructor
	//
	PointLight::PointLight(Vec3f pos, Vec3f intensity, std::string type):LightSource(pos, intensity, type) {

	}


	/**
	* Prints light data
	* this function implements the abstract function of the base LightSource class
	*/
	void PointLight::printLight() {
		printf("I am a point light source! \n");
		printf("xpos: %f, ypos: %f, zpos: %f, xintensity: %f, yintensity: %f, zintensity: %f\n", pos.x, pos.y, pos.z, intensity.z, intensity.y, intensity.z);
	}



} //namespace rt