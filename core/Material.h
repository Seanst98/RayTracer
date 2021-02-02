/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/geometry.h"

namespace rt{

class Material{
public:

	float ks;
	float kd;
	float specularexponent;
	Vec3f diffuseColour;
	unsigned char* texture;
	int textureWidth;
	int textureHeight;
	int channels;
	bool isGlass;


private:
};


} //namespace rt



#endif /* MATERIAL_H_ */
