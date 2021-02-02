/*
 * ThinLens.h
 *
 */

#ifndef THINLENS_H_
#define THINLENS_H_

#include "core/Camera.h"


namespace rt{

class ThinLens:public Camera{
public:

	//
	// Constructors
	//
	ThinLens():Camera(){};
	ThinLens(int width, int height, int fov, float ap, float f, std::string newsampling, Vec3f pos, Vec3f lookAt, std::string newtype);

	//
	//Destructor
	//
	~ThinLens(){};

	//
	// print function (implementing abstract function of base class)
	//
	void printCamera();


	float ap;
	float f;
	std::string sampling;


private:

};



} //namespace rt




#endif /* THINLENS_H_ */
