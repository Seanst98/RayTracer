/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"
#include "materials/BlinnPhong.h"
#include <algorithm>
#include "RayHitStructs.h"
#include <random>
#include <cstdlib>
#include "cameras/ThinLens.h"
#include "lights/AreaLight.h"

namespace rt{


/**
 * Performs ray tracing to render a photorealistic scene
 *
 * @param camera the camera viewing the scene
 * @param scene the scene to render, including objects and lightsources
 * @param nbounces the number of bounces to consider for raytracing
 *
 * @return a pixel buffer containing pixel values in linear RGB format
 */
Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces){

	Vec3f* pixelbuffer=new Vec3f[camera->getWidth()*camera->getHeight()];

	//----------main rendering function to be filled------

	// for each pixel of the image
	for (int i = 0; i < camera->getHeight(); i++) {
		std::cout << i << std::endl;
		for (int j = 0; j < camera->getWidth(); j++) {
			//Compute ray direction
			Ray* primRay = new Ray();
			int k=0;
			Vec3f temp;

			if (camera->getType().compare("thinlens") == 0) {
				for (k = 0; k < scene->nSamples; k++) {
					computePrimRay(i, j, primRay, camera, k, scene->nSamples);
					temp = temp + trace(primRay, nbounces, scene);
				}
				temp.x = temp.x / k;
				temp.y = temp.y / k;
				temp.z = temp.z / k;
				pixelbuffer[(i * camera->getWidth()) + j] = temp;
				delete primRay;
			}
			else {
				computePrimRay(i, j, primRay, camera, k, scene->nSamples);
				temp = temp + trace(primRay, nbounces, scene);
				pixelbuffer[(i * camera->getWidth()) + j] = temp;
				delete primRay;
			}

			//pixelbuffer[(i * camera->getWidth()) + j] = trace(primRay, nbounces, scene);
		}
	}

	return pixelbuffer;

}

void RayTracer::computePrimRay(int i, int j, Ray* ray, Camera* camera, int k, int nSamples) {

	float randR = 0;
	float randA = 0;
	float f = 0;
	float ap = 0;

	float imageAspectRatio = camera->getWidth() / camera->getHeight();
	float Px = camera->getLookAt().x - camera->getPos().x + (2 * ((i + 0.5) / camera->getWidth()) - 1) * tan(camera->getFov() / 2 * 3.14159265358979323846264338327950288 / 180) * imageAspectRatio;
	float Py = camera->getLookAt().y - camera->getPos().y + (1 - 2 * ((j + 0.5) / camera->getHeight())) * tan(camera->getFov() / 2 * 3.14159265358979323846264338327950288 / 180);

	ray->origin = camera->getPos();

	ray->direction = Vec3f(Px - camera->getLookAt().x, Py - camera->getLookAt().y, camera->getLookAt().z) - ray->origin;

	ray->direction = ray->direction.normalize();

	if (camera->getType().compare("thinlens") == 0) {

		ThinLens* thinLens = (ThinLens*)camera;

		f = thinLens->f;
		ap = thinLens->ap;

		if (thinLens->sampling.compare("jittering")==0) {

			float APRstep = (ap/2) / nSamples;
			float APAstep = 360.0 / nSamples;

			float a = (k) * APAstep;
			float b = (2*(double)std::rand() / (double)RAND_MAX) - 1;
			b = std::fmod(b, APAstep);

			float c = (k) * APRstep;
			float d = (2 * (double)std::rand() / (double)RAND_MAX) - 1;
			d = std::fmod(d, APRstep);

			randA = a + b;
			randR = c + d;

		}
		else {
			randR = std::rand();
			randR = std::fmod(randR, (ap / 2));
			randA = (double)std::rand() / RAND_MAX;
			randA = std::fmod(randA*360.0, 360.0);
		}


		Vec3f focalPoint;
		focalPoint = ray->origin + (f * ray->direction);

		Vec3f aperturePoint;
		aperturePoint.x = randR * cos(randA);
		aperturePoint.y = randR * sin(randA);
		aperturePoint.z = ray->origin.z;

		ray->origin = aperturePoint;

		ray->direction = focalPoint - ray->origin;
		ray->direction = ray->direction.normalize();

	}


}


Vec3f RayTracer::trace(const Ray* ray, int nbounces, Scene* scene) {
	Shape* shape = nullptr;
	float minDist = std::numeric_limits<float>::max();
	Vec3f nHit;  //Normal to the point of intersection
	Hit h;
	Hit finalHit;

	for (int i = 0; i < scene->shapes.size(); i++) {

		h = scene->shapes[i]->intersect(*ray);

		if (h.hit == false) {  //No hit
			continue;
		}
		nHit = h.normal;
		nHit = nHit.normalize();

		Vec3f temp = (ray->origin - h.point) * (ray->origin - h.point);
		float temp2 = temp.x + temp.y + temp.z;
		float distance = sqrt(temp2);

		if (distance < minDist) {
			minDist = distance;
			shape = h.shape;
			finalHit = h;
		}
	}

	if (shape == nullptr) {
		return scene->backGroundColour*100;
	}

	Vec3f surfaceColour = shape->getMaterial()->diffuseColour;
	nHit = finalHit.normal;
	nHit = nHit.normalize();
	Vec3f color;

	if (nbounces > 0) {
		if (shape->getMaterial()->isGlass) {

			//Compute relfection Ray
			Ray* reflectionRay = new Ray();
			float reflectTemp = ray->direction.dotProduct(nHit);
			reflectionRay->direction = ray->direction - (2 * reflectTemp) * nHit;
			reflectionRay->direction = reflectionRay->direction.normalize();
			reflectionRay->origin = finalHit.point + (reflectionRay->direction * 1e-4);

			color = 0.8 * trace(reflectionRay, nbounces-1, scene);

			//Compute refraction Ray
			Ray* refractionRay = new Ray();
			float refractTemp = ray->direction.dotProduct(nHit);
			float discriminant = 1 - (0.4444 * (1 - refractTemp));
			if (discriminant > 0) {
				refractionRay->direction = 1.5 * (ray->direction - nHit * refractTemp) - nHit * sqrt(discriminant);
				color = color + trace(refractionRay, nbounces-1, scene);
				color.x = color.x / 2;
				color.y = color.y / 2;
				color.z = color.z / 2;
				delete reflectionRay;
				delete refractionRay;
				return color;
			}

			delete reflectionRay;
			delete refractionRay;
			return color;

		}
	}

	//***********************************
	//If a shape emits
	//Not for marking, just something I may add later
	//***********************************
	/*for (unsigned i = 0; i < scene->shapes.size(); i++) {
		//if (scene->shapes[i].emmissionColour> 0)

		//For if a shape emitts light

		Vec3f transmission = 1;
		Ray lightDirection;
		lightDirection.direction = scene->shapes[i]->getPos() - finalHit.point;
		//lightDirection.direction = finalHit.point - scene->shapes[i]->getPos();
		lightDirection.direction = lightDirection.direction.normalize();
		lightDirection.origin = finalHit.point + nHit + (finalHit.point + nHit)*1e-4;

		for (unsigned j = 0; j < scene->shapes.size(); j++) {
			if (i != j) {
				Hit lightHit = scene->shapes[j]->intersect(lightDirection);
				if (lightHit.hit) {
					transmission = 0;
					break;
				}
			}
		}

		surfaceColour = surfaceColour + (scene->shapes[i]->getMaterial()->diffuseColour * transmission
			* std::max(float(0), nHit.dotProduct(lightDirection.direction)) * 1);// scene->shapes[i]->emmissionColour;
	}*/

	Hit shadowh;
	shadowh.hit = false;
	Vec3f shade = 0;

	bool isShadow = false;
	Ray shadowRay;

	//For all lights sum the shading result
	for (int i = 0; i < scene->lightSources.size(); i++) {
		isShadow = false;

		if (scene->lightSources[i]->getType().compare("arealight") == 0) {

			AreaLight* areaLight = (AreaLight*)scene->lightSources[i];

			Vec3f sum;
			Vec3f sumIntensity;


			if (scene->sampling.compare("jittering") == 0) {
				for (int k = 0; k < scene->nSamples; k++) {
					isShadow = false;

					float ALRstep = (2*areaLight->radius) / scene->nSamples;
					float ALAstep = 360.0 / scene->nSamples;
					std::random_device rd;
					std::mt19937 mt(rd());
					std::uniform_real_distribution<double> dist(-ALRstep, ALRstep);
					std::uniform_real_distribution<double> distA(0, 360.0);


					Vec3f lightPoint;

					//Calculate jittered point using step and random jitter addition
					float a = (areaLight->radius - (ALRstep * k)) + dist(mt);
					float b = (areaLight->radius - (ALRstep * k)) + dist(mt);
					float c = (areaLight->radius - (ALRstep * k)) + dist(mt);

					float d = (ALAstep * k) + distA(mt);


					lightPoint.x = a * cos(d);
					lightPoint.y = b * sin(d);
					lightPoint.z = c * tan(d);




					lightPoint = areaLight->getPos() + lightPoint;
					
					shadowRay.direction = lightPoint - finalHit.point;
					shadowRay.direction = shadowRay.direction.normalize();
					shadowRay.origin = finalHit.point + (shadowRay.direction * 1e-4);

					//Check shadow ray against all shapes
					for (int j = 0; j < scene->shapes.size(); j++) {
						shadowh = scene->shapes[j]->intersect(shadowRay);

						if (shadowh.hit) {
							isShadow = true;
						}

					}

					if (!isShadow) {
						// point is illuminated so calculate blinn phong across all light sources
						BlinnPhong blinnPhong;
						sum = sum + blinnPhong.calculateShade(nHit, finalHit.point, shadowRay.direction, ray->direction, shape, scene->lightSources[i]->getIntensity());

					}

				}

				sum.x = sum.x / scene->nSamples;
				sum.y = sum.y / scene->nSamples;
				sum.z = sum.z / scene->nSamples;

				shade = shade + sum;

			}
			else {
				float count = 0;
				for (int k = 0; k < scene->nSamples; k++) {
					isShadow = false;

					std::random_device rd;
					std::mt19937 mt(rd());
					std::uniform_real_distribution<double> dist(-1.0, 1.0);


					Vec3f lightPoint;

					float a = dist(mt);
					float b = dist(mt);
					float c = dist(mt);

					lightPoint.x = areaLight->getPos().x + (areaLight->radius * (a));
					lightPoint.y = areaLight->getPos().y + (areaLight->radius * (b));
					lightPoint.z = areaLight->getPos().z + (areaLight->radius * (c));

					shadowRay.direction = lightPoint - finalHit.point;
					//shadowRay.direction = (areaLight->getPos() - finalHit.point) + lightPoint;
					shadowRay.direction = shadowRay.direction.normalize();
					shadowRay.origin = finalHit.point + (shadowRay.direction * 1e-4);

					//Check shadow ray against all shapes
					for (int j = 0; j < scene->shapes.size(); j++) {
						shadowh = scene->shapes[j]->intersect(shadowRay);

						if (shadowh.hit) {
							isShadow = true;
						}

					}

					if (!isShadow) {
						count++;
						// point is illuminated so calculate blinn phong across all light sources
						//std::cout << count << std::endl;
						BlinnPhong blinnPhong;
						Vec3f temp = blinnPhong.calculateShade(nHit, finalHit.point, shadowRay.direction, ray->direction, shape, scene->lightSources[i]->getIntensity());
						sum = sum + (1.0/(double)scene->nSamples)*blinnPhong.calculateShade(nHit, finalHit.point, shadowRay.direction, ray->direction, shape, scene->lightSources[i]->getIntensity());
						sumIntensity = sumIntensity + areaLight->getIntensity();
					}

				}


				shadowRay.direction = scene->lightSources[i]->getPos() - finalHit.point;
				shadowRay.direction = shadowRay.direction.normalize();
				shadowRay.origin = finalHit.point + (shadowRay.direction * 1e-4);

				BlinnPhong blinnPhong;
				sumIntensity.x = sumIntensity.x / scene->nSamples;
				sumIntensity.y = sumIntensity.y / scene->nSamples;
				sumIntensity.z = sumIntensity.z / scene->nSamples;
				shade = shade + blinnPhong.calculateShade(nHit, finalHit.point, shadowRay.direction, ray->direction, shape, sumIntensity);

			}

			//If the shade has gone over the max then set to max
			if (shade.x > 255) { shade.x = 255; }
			if (shade.y > 255) { shade.y = 255; }
			if (shade.z > 255) { shade.z = 255; }


		}
		else {

			shadowRay.direction = scene->lightSources[i]->getPos() - finalHit.point;
			shadowRay.direction = shadowRay.direction.normalize();
			shadowRay.origin = finalHit.point + (shadowRay.direction * 1e-4);
			int count = 0;

			//Check shadow ray against all shapes
			for (int j = 0; j < scene->shapes.size(); j++) {
				shadowh = scene->shapes[j]->intersect(shadowRay);

				if (shadowh.hit) {
					isShadow = true;
				}

			}


			if (!isShadow) {
				// point is illuminated so calculate blinn phong across all light sources
				BlinnPhong blinnPhong;
				shade = shade + blinnPhong.calculateShade(nHit, finalHit.point, shadowRay.direction, ray->direction, shape, scene->lightSources[i]->getIntensity());
				shade = shade + color;

				//If the shade has gone over the max then set to max
				if (shade.x > 255) { shade.x = 255; }
				if (shade.y > 255) { shade.y = 255; }
				if (shade.z > 255) { shade.z = 255; }
			}

		}

	}

	return shade;


}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
 *
 * @return the tonemapped image
 */
Vec3f* RayTracer::tonemap(Vec3f* pixelbuffer){

	//---------tonemapping function to be filled--------





	return pixelbuffer;

}





} //namespace rt


