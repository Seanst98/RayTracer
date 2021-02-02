/*
 * Camera.cpp
 *
 */
#include <iostream>

#include "Camera.h"
#include "cameras/Pinhole.h"
#include "cameras/ThinLens.h"

namespace rt{

	Camera::~Camera(){};



/**
 * Factory function that returns camera subclass based on camera specifications
 *
 * @param cameraSpecs camera specifications json object
 *
 * @return camera subclass instance
 *
 */
Camera* Camera::createCamera(Value& cameraSpecs){

	//check if cameratype is defined

	if (!cameraSpecs.HasMember("type")){
		std::cerr<<"Camera type not specified"<<std::endl;
		exit(-1);
	}

	std::string cameraType=cameraSpecs["type"].GetString();

	//return camera object based on camera specs
	if (cameraType.compare("pinhole")==0){

		//Retreive the position vector
		Vec3f pos;
		const Value& posArray = cameraSpecs["position"];
		assert(posArray.IsArray());

		//Set position vector
		pos.x = posArray[0].GetFloat(); pos.y = posArray[1].GetFloat(); pos.z = posArray[2].GetFloat();


		//Retrieve the lookAt vector
		Vec3f lookAt;
		const Value& lookArray = cameraSpecs["lookAt"];
		assert(lookArray.IsArray());

		//Set the lookAt vector
		lookAt.x = lookArray[0].GetFloat(); lookAt.y = lookArray[1].GetFloat(); lookAt.z = lookArray[2].GetFloat();

		return new Pinhole(cameraSpecs["width"].GetInt(),
				cameraSpecs["height"].GetInt(),
				cameraSpecs["fov"].GetInt(),
				pos,
				lookAt, cameraType);

	}else if (cameraType.compare("thinlens")==0){

		//Retreive the position vector
		Vec3f pos;
		const Value& posArray = cameraSpecs["position"];
		assert(posArray.IsArray());

		//Set position vector
		pos.x = posArray[0].GetFloat(); pos.y = posArray[1].GetFloat(); pos.z = posArray[2].GetFloat();

		//Retrieve the lookAt vector
		Vec3f lookAt;
		const Value& lookArray = cameraSpecs["lookAt"];
		assert(lookArray.IsArray());

		//Set the lookAt vector
		lookAt.x = lookArray[0].GetFloat(); lookAt.y = lookArray[1].GetFloat(); lookAt.z = lookArray[2].GetFloat();

		return new ThinLens(cameraSpecs["width"].GetInt(),
			cameraSpecs["height"].GetInt(),
			cameraSpecs["fov"].GetInt(),
			cameraSpecs["f"].GetFloat(),
			cameraSpecs["ap"].GetFloat(),
			cameraSpecs["sampling"].GetString(),
			pos,
			lookAt, cameraType);
	}

	return 0;

}



} //namespace rt
