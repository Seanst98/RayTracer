/*
 * LightSource.cpp
 *
 */
#include <iostream>
#include "LightSource.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

namespace rt {

	/**
	 * Factory function that returns LightSource subclass based on light specifications
	 *
	 * @param lightSpecs light specifications json object
	 *
	 * @return LightSource subclass instance
	 *
	 */
	LightSource* LightSource::createLightSource(Value& lightSpecs) {

		//Check if light type is defined

		if (!lightSpecs.HasMember("type")) {
			std::cerr << "Light type not specified" << std::endl;
			exit(-1);
		}

		std::string lightType = lightSpecs["type"].GetString();

		//Return LightSource object based on light specs
		if (lightType.compare("pointlight") == 0) {

			//Retreive the position vector
			Vec3f pos;
			const Value& posArray = lightSpecs["position"];
			assert(posArray.IsArray());

			//Set position vector
			pos.x = posArray[0].GetFloat(); pos.y = posArray[1].GetFloat(); pos.z = posArray[2].GetFloat();


			//Retrieve the intensity vector
			Vec3f intensity;
			const Value& intensityArray = lightSpecs["intensity"];
			assert(intensityArray.IsArray());

			intensity.x = intensityArray[0].GetFloat(); intensity.y = intensityArray[1].GetFloat(); intensity.z = intensityArray[2].GetFloat();

			return new PointLight(pos, intensity, lightType);

		}
		else if (lightType.compare("arealight") == 0) {

			//Retreive the position vector
			Vec3f pos;
			const Value& posArray = lightSpecs["position"];
			assert(posArray.IsArray());

			//Set position vector
			pos.x = posArray[0].GetFloat(); pos.y = posArray[1].GetFloat(); pos.z = posArray[2].GetFloat();


			//Retrieve the intensity vector
			Vec3f intensity;
			const Value& intensityArray = lightSpecs["intensity"];
			assert(intensityArray.IsArray());

			intensity.x = intensityArray[0].GetFloat(); intensity.y = intensityArray[1].GetFloat(); intensity.z = intensityArray[2].GetFloat();

			float radius = lightSpecs["radius"].GetFloat();

			return new AreaLight(pos, intensity, lightType, radius);
		}

		return 0;

	}

} //namespace rt