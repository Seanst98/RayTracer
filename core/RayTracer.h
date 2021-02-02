/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "math/geometry.h"
#include "core/Camera.h"
#include "core/Scene.h"

namespace rt{



/*
 * Raytracer class declaration
 */
class RayTracer {
public:

	RayTracer();

	static Vec3f* render(Camera* camera, Scene* scene, int nbounces);
	static Vec3f* tonemap(Vec3f* pixelbuffer);
	static void computePrimRay(int i, int j, Ray* primRay, Camera* camera, int k, int nSamples);
	static Vec3f trace(const Ray* ray, int nbounces, Scene* scene);


	static long long int numIntersectChecks;

private:

};




} //namespace rt



#endif /* RAYTRACER_H_ */

