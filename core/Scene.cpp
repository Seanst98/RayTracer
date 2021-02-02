/*
 * Scene.cpp
 *
 */
#include "Scene.h"
#include "shapes/BVH.h"




namespace rt{

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value& scenespecs){

	//----------parse json object to populate scene-----------

	//

	//Get array of light sources
	//

	//check if there is a lightsource
	if (!scenespecs.HasMember("lightsources")) {
		std::cout << "Light source not specified" << std::endl;
	}


	//Retrieve the lightsources
	Value& lightsources = scenespecs["lightsources"];
	assert(lightsources.IsArray());

	for (SizeType i = 0; i < lightsources.Size(); i++) {
		LightSource* lightSource = LightSource::createLightSource(lightsources[i]);
		lightSources.push_back(lightSource);
		lightSource->printLight();
	}

	//Retrieve the shapes
	Value& shapesArray = scenespecs["shapes"];
	assert(shapesArray.IsArray());

	for (SizeType i = 0; i < shapesArray.Size(); i++) {
		Shape* shape = Shape::createShape(shapesArray[i]);
		shapes.push_back(shape);
	}

	//If BVH is specified then construct a BVH and use it as the only shape stored
	Value& BVHSpecs = scenespecs["BVH"];
	if (BVHSpecs["use"].GetBool()) {
		BVH* bvh = new BVH(shapes);
		shapes.clear();
		shapes.push_back(bvh);
	}

	//Retrieve background colour
	Value& backColourArray = scenespecs["backgroundcolor"];
	assert(backColourArray.IsArray());

	//Set back ground colour vector
	backGroundColour.x = backColourArray[0].GetFloat();
	backGroundColour.y = backColourArray[1].GetFloat();
	backGroundColour.z = backColourArray[2].GetFloat();

	//Set nSamples
	nSamples = scenespecs["nsamples"].GetInt();

	//Set sampling
	sampling = scenespecs["sampling"].GetString();
}



} //namespace rt
