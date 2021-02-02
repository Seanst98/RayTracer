/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace rt{

	Vec3f BlinnPhong::calculateShade(Vec3f nHit, Vec3f hitPoint, Vec3f shadowDirection, Vec3f rayDirection, Shape* shape, Vec3f lightIntensity) {

		int width = 0, height = 0, channels = 0;
		unsigned char* data = shape->getMaterial()->texture;
		
		Vec2f uvVec = shape->getTextureCoordinates(hitPoint);
		float shadeTemp = nHit.dotProduct(shadowDirection);

		Vec3f shade;

		if (data!=nullptr) {
			int u = (int)uvVec.x;
			int v = (int)uvVec.y;
			unsigned char* pixelOffset = data + (u + shape->getMaterial()->textureWidth * v) * shape->getMaterial()->channels;

			unsigned char r = pixelOffset[0];
			unsigned char g = pixelOffset[1];
			unsigned char b = pixelOffset[2];

			Vec3f colour = Vec3f(r, g, b);
			colour = colour.normalize();

			shade = (colour * shape->getMaterial()->kd * lightIntensity * (std::max(float(0), nHit.dotProduct(shadowDirection))));

		}
		else {
			shade = (shape->getMaterial()->diffuseColour * shape->getMaterial()->kd * lightIntensity * (std::max(float(0), nHit.dotProduct(shadowDirection))));

		}

		//Blinn-Phong
		Vec3f blinn = -rayDirection + shadowDirection;
		Vec3f blinnH = blinn.normalize();
		Vec3f temp = shape->getMaterial()->ks * lightIntensity;
		float temp1 = std::max(float(0), nHit.dotProduct(blinnH));
		temp1 = std::pow(temp1, shape->getMaterial()->specularexponent);
		Vec3f spec = temp * temp1;
		Vec3f specLight = shape->getMaterial()->ks * lightIntensity * (std::pow(std::max(float(0), nHit.dotProduct(blinnH)), shape->getMaterial()->specularexponent));

		shade = shade + specLight;


		//If the shade has gone over the max then set to max
		if (shade.x > 255) { shade.x = 255; }
		if (shade.y > 255) { shade.y = 255; }
		if (shade.z > 255) { shade.z = 255; }

		return shade;

	}




} //namespace rt


