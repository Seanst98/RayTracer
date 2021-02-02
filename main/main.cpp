/*
 * main.cpp
 *
 */

#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "math/geometry.h"
#include "parsers/PPMWriter.h"

#include "core/RayTracer.h"
#include "core/Camera.h"
#include "core/Scene.h"

#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION


using namespace rt;
using namespace rapidjson;

long long int RayTracer::numIntersectChecks = 0;

int main(int argc, char* argv[]){

	//Use example until have working ray tracer
	std::string jsonfile = "../examples/example.json";

	//parse commandline arguments
	char* inputFile=argv[1];    //first command line argument holds the path to the json input file
	char* outputFile=argv[2];   //second command line argument holds the path to the output image file

	outputFile = "output.ppm";

	std::printf("Input file: %s\n",inputFile);
	//std::printf("Input file: %s\n", jsonfile);

	//parse json input file using rapidjson
	//std::ifstream ifs(inputFile);
	std::ifstream ifs(jsonfile);
	IStreamWrapper is(ifs);
	Document d;     // d holds the complete json object
	d.ParseStream(is);

	//generate a camera according to the input file
	Camera* camera=Camera::createCamera(d["camera"]);

	//print camera data (based on the input file provided)
	camera->printCamera();

	//generate the scene according to the input file
	Scene* scene=new Scene();
	scene->createScene(d["scene"]);

	//
	// Main function, render scene
	//
	/*int step = 10;
	Vec3f* pixelbuffer = new Vec3f[camera->getWidth() * camera->getHeight()];
	int width = 0, height = 0, channels = 0;
	std::string refString = "thin lens 5k jittering.ppm";
	unsigned char* data = stbi_load(refString.c_str(), &width, &height, &channels, 0);

	do {
		scene->nSamples = step;
		long float sum = 0;

		pixelbuffer = RayTracer::render(camera, scene, d["nbounces"].GetInt());

		for (int i = 0; i < (camera->getWidth() * camera->getHeight()); i++) {
			unsigned char* pixelOffset = data + (i) * channels;
			unsigned char r = pixelOffset[0];
			unsigned char g = pixelOffset[1];
			unsigned char b = pixelOffset[2];

			if (i == 205000) {
				std::cout << "here" << std::endl;
			}

			Vec3f colour = Vec3f(r, g, b);

			float R = std::abs((double)r/255.0 - (double)pixelbuffer[i].x/255.0);
			R = R * R;

			float G = std::abs((double)g/255.0 - (double)pixelbuffer[i].y/255.0);
			G = G * G;

			float B = std::abs((double)b/255.0 - (double)pixelbuffer[i].z/255.0);
			B = B * B;

			sum = sum + (R + G + B);
		}

		float MSE = ((double)1 / ((double)camera->getWidth() * (double)camera->getHeight()));
		MSE = MSE * sum;

		std::cout << "Step: " << step << " MSE: " << MSE << std::endl;

		step += 50;
	} while (step < 2000);*/

	Vec3f* pixelbuffer = RayTracer::render(camera, scene, d["nbounces"].GetInt());


	std::cout << "Number of intersection tests: " << RayTracer::numIntersectChecks << std::endl;



	//free resources when rendering is finished
	delete scene;



	//convert linear RGB pixel values [0-1] to range 0-255
	RayTracer::tonemap(pixelbuffer);



	std::printf("Output file: %s\n",outputFile);

	//write rendered scene to file (pixels RGB values must be in range 0255)
	PPMWriter::PPMWriter(camera->getWidth(), camera->getHeight(), pixelbuffer, outputFile);

	delete pixelbuffer;
	delete camera;
}


