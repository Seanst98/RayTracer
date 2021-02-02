#
# Computer Graphics, Autumn 2019
#

This is the template code supplied for the 2nd assignment of the Computer Graphics course, Autumn 2019, at University of Edinburgh. It dictates the class structure to be followed for the implementation of this assignment. 

Example code for the provided libraries can be found in the examples/ folder, along with an example input file example.json.

The skeleton of the main() function has already been defined, as well as some inheritance examples of the Camera class and its subclasses, and the Sphere class, as a child of the Shape class,  but you can add or modify the functions used, as long as the class structure is respected.


To compile the code:

cd path/to/RayTracer
mkdir build
cmake ..
make


This compiles both the raytracer and the example programs, executables of which can be found in build/ folder.

To run the examples, do:

1. for the json parsing example:

./jsonexample ../examples/example.json

2. for the vector library example:

./vectorexample

3. for the raytracer provided:

./raytracer ../examples/example.json <path_to_output_img>/testout.ppm

The raytracer provided outputs an empty file. You will need to implement the actual raytracer. Good luck!