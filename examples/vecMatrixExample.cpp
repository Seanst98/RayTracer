/*
 * vecMatricExample.cpp
 *
 * These are the values for the input matrix:
 * 0.707107 0 -0.707107 0
 * -0.331295 0.883452 -0.331295 0
 * 0.624695 0.468521 0.624695 0
 * 4.000574 3.00043 4.000574 1
 *
 * Given the input matrix, the inverse matrix computed by our code should match the following values:
 * 0.707107 -0.331295 0.624695 0
 * 0 0.883452 0.468521 0
 * -0.707107 -0.331295 0.624695
 *  0 0 0 -6.404043 1
 */
// Copyright (C) 2012  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <iostream>

#include "math/geometry.h"


int main(int argc, char **argv)
{

//define vector
Vec3f v(0, 1, 2);


std::cerr <<"Example vector:"<< v << std::endl;


Matrix44f a, b, c;
c = a * b;

Matrix44f d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1);

std::cerr<<"Example matrix:"<<std::endl;
std::cerr << d << std::endl;
d.invert();
std::cerr<<"Inverse of above matrix:"<<std::endl;
std::cerr << d << std::endl;

return 0;
}

