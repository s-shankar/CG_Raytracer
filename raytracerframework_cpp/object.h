//
//  Framework for a raytracer
//  File: object.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"
#include <Eigen/Dense>

class Material;
class Intersection;
class Plane;

class Object {
public:
	Material *material;
	double theta = 0.0, phi = 0.0;

	virtual ~Object() { }

	virtual Hit intersect(const Ray &ray) = 0;
	virtual Intersection distanceToPlane(const Plane &plane) = 0;
	virtual void changeBase(const Eigen::Matrix4d &changeOfBaseMatrix) = 0;
	virtual Color getColor(Point p) = 0;
	void setRotate(double r1, double r2, double r3)
	{
		theta = r1;
		phi = r2 + r3;
	}
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
