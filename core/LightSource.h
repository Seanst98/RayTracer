/*
 * LightSource.h
 *
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "rapidjson/document.h"
#include "math/geometry.h"


using namespace rapidjson;

namespace rt{

class LightSource{
public:

	//
	// Constructors
	//
	LightSource() {}
	LightSource(Vec3f pos, Vec3f intensity, std::string type) :pos(pos), intensity(intensity), type(type){};

	//
	// Destructor
	//
	virtual ~LightSource() {};

	//
	// factory function : returns light source instance dynamically based on light source specifications
	//
	static LightSource* createLightSource(Value& lightSpecs);

	//
	// print function (to be implemented by the subclasses )
	//
	virtual void printLight() = 0;



	//
	// Getters and Setters
	//

	Vec3f getPos() {
		return pos;
	}

	void setPos(Vec3f newPos) {
		this->pos = newPos;
	}

	Vec3f getIntensity() {
		return intensity;
	}

	void setIntensity(Vec3f newIntensity) {
		this->intensity = newIntensity;
	}

	std::string getType() {
		return type;
	}


private:

protected:
	//
	//Light source members
	//
	Vec3f pos;  //World position of light source
	Vec3f intensity;  //Intensity of light RGB
	std::string type;

};

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
