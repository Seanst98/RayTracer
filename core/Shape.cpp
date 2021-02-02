/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/Sphere.h"
#include "shapes/Plane.h"
#include "shapes/Triangle.h"
#include "shapes/TriMesh.h"
#include <iostream>
#include <vector>
#include "stb_image.h"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION



namespace rt{

	/**
	 * Factory function that returns Shape subclass based on shape specifications
	 *
	 * @param shapeSpecs shape specifications json object
	 *
	 * @return Shape subclass instance
	 *
	 */
	
	Shape* Shape::createShape(Value& shapeSpecs) {

	//Check if shape type is defined

	if (!shapeSpecs.HasMember("type")) {
		std::cerr << "Shape type not specified" << std::endl;
		exit(-1);
	}

	std::string shapeType = shapeSpecs["type"].GetString();

	//Return Shape object based on shape specs
	if (shapeType.compare("sphere") == 0) {

		//Retreive the position vector
		Vec3f centre;
		const Value& posArray = shapeSpecs["center"];
		assert(posArray.IsArray());

		//Set position vector
		centre.x = posArray[0].GetFloat(); centre.y = posArray[1].GetFloat(); centre.z = posArray[2].GetFloat();


		//Retrieve the radius
		float radius = shapeSpecs["radius"].GetFloat();

		Material* material = retrieveMaterial(shapeSpecs);

		return new Sphere(centre, radius, material);

	}
	
	else if (shapeType.compare("plane") == 0) {

		//Retrieve the points
		std::vector<Vec3f> points;
		const Value& pointsArray = shapeSpecs["points"];
		assert(pointsArray.IsArray());

		for (SizeType i = 0; i < pointsArray.Size(); i++) {
			const Value& pointTemp = pointsArray[i];
			points.push_back(Vec3f(pointTemp[0].GetFloat(), pointTemp[1].GetFloat(), pointTemp[2].GetFloat()));
		}

		Material* material = retrieveMaterial(shapeSpecs);

		return new Plane(points, material);

	}

	else if (shapeType.compare("triangle") == 0) {

		//Retrieve the points
		std::vector<Vec3f> points;
		const Value& pointsArray = shapeSpecs["points"];
		assert(pointsArray.IsArray());

		for (SizeType i = 0; i < pointsArray.Size(); i++) {
			const Value& pointTemp = pointsArray[i];
			points.push_back(Vec3f(pointTemp[0].GetFloat(), pointTemp[1].GetFloat(), pointTemp[2].GetFloat()));
		}

		Material* material = retrieveMaterial(shapeSpecs);

		return new Triangle(points, material);
	}

	else if (shapeType.compare("trimesh") == 0) {

		//Retrieve the points
		std::vector<Vec3f> points;
		const Value& pointsArray = shapeSpecs["points"];
		assert(pointsArray.IsArray());

		for (SizeType i = 0; i < pointsArray.Size(); i++) {
			const Value& pointTemp = pointsArray[i];
			points.push_back(Vec3f(pointTemp[0].GetFloat(), pointTemp[1].GetFloat(), pointTemp[2].GetFloat()));
		}

		Material* material = retrieveMaterial(shapeSpecs);

		//Retrieve the texture points
		std::vector<Vec2f> textpoints;
		const Value& textpointsArray = shapeSpecs["texturecoordinates"];
		assert(textpointsArray.IsArray());

		for (SizeType i = 0; i < textpointsArray.Size(); i++) {
			const Value& pointTemp = textpointsArray[i];
			textpoints.push_back(Vec2f(pointTemp[0].GetFloat(), pointTemp[1].GetFloat()));
		}

		return new TriMesh(points, material, textpoints);


	}
	
	return 0;

}


Material* Shape::retrieveMaterial(Value& shapeSpecs) {
	//Retrieve material properties
	const Value& materialSpecs = shapeSpecs["material"];
	Material* material = new Material();
	material->ks = materialSpecs["ks"].GetFloat();
	material->kd = materialSpecs["kd"].GetFloat();
	material->specularexponent = materialSpecs["specularexponent"].GetFloat();
	Vec3f diffcolour;
	const Value& diffArray = materialSpecs["diffusecolor"];
	assert(diffArray.IsArray());
	diffcolour.x = diffArray[0].GetFloat(); diffcolour.y = diffArray[1].GetFloat(); diffcolour.z = diffArray[2].GetFloat();
	material->diffuseColour = Vec3f(diffcolour.x, diffcolour.y, diffcolour.z);

	std::string textureString = materialSpecs["texture"].GetString();

	int width = 0, height = 0, channels = 0;
	unsigned char* data = stbi_load(textureString.c_str(), &width, &height, &channels, 0);
	material->texture = data;

	material->textureHeight = height;
	material->textureWidth = width;
	material->channels = channels;

	std::string glassString = materialSpecs["isGlass"].GetString();

	material->isGlass = false;
	if (glassString.compare("true") == 0) {
		material->isGlass = true;
	}





	return material;
}



} //namespace rt