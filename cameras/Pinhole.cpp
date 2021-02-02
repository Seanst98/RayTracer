/*
 * Pinhole.cpp
 *
 *
 */
#include <iostream>

#include "Pinhole.h"
#include "math/geometry.h"



namespace rt{

	//
	// Pinhole constructor (example)
	//
	Pinhole::Pinhole(int width, int height, int fov, Vec3f pos, Vec3f lookAt, std::string newtype):Camera(width, height, fov, pos, lookAt, newtype){


		// to fill


	}


	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void Pinhole::printCamera(){
		printf("I am a pinhole camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d, xpos: %f, ypos: %f, zpos: %f, xlookAt: %f, ylookAt: %f, zlookAt: %f\n", width, height, fov, pos.x, pos.y, pos.z, lookAt.z, lookAt.y, lookAt.z);
	}



} //namespace rt

