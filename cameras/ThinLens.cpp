/*
 * ThinLens.cpp
 *
 *
 */
#include <iostream>


#include "ThinLens.h"




namespace rt{

	//
	// ThinLens constructor (example)
	//
	ThinLens::ThinLens(int width, int height, int fov, float newf, float newap, std::string newsampling, Vec3f pos, Vec3f lookAt, std::string newtype):Camera(width, height, fov, pos, lookAt, newtype){

		//to fill
		f = newf;
		ap = newap;
		sampling = newsampling;
	}

	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void ThinLens::printCamera(){
		printf("I am a thin lens camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
	}

} //namespace rt
