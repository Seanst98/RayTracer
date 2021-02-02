/*
 * AreaLight.cpp
 *
 *
 */
#include "AreaLight.h"




namespace rt{

	//
	//AreaLight constructor
	//
	AreaLight::AreaLight(Vec3f pos, Vec3f intensity, std::string type, float newradius):LightSource(pos, intensity, type) {
		radius = newradius;
	}

	/**
	* Prints light data
	* this function implements the abstract function of the base LightSource class
	*/
	void AreaLight::printLight() {
		printf("I am an area light source! \n");
		printf("xpos: %f, ypos: %f, zpos: %f, xintensity: %f, yintensity: %f, zintensity: %f\n", pos.x, pos.y, pos.z, intensity.z, intensity.y, intensity.z);
	}



} //namespace rt