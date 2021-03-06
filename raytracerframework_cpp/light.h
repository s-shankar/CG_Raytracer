//
//  Framework for a raytracer
//  File: light.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    adjustment of Hit class by Olivier Wavrin
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef LIGHT_H_PG2BAJRA
#define LIGHT_H_PG2BAJRA

#include <iostream>
#include <limits>
#include "triple.h"
#include <Eigen/Dense>

class Light
{
public:
	Light(Point pos, Color c) : position(pos), color(c)
	{ }

	void changeBase(const Eigen::Matrix4d &changeOfBaseMatrix)
	{
		position = (Point)position.matrixProduct(changeOfBaseMatrix);
	}

	Point position;
	Color color;
};

class Ray
{
public:
	Point O;
	Vector D;

	Ray(const Point &from, const Vector &dir)
		: O(from), D(dir)
	{ }

	/*void changeBase(const Eigen::Matrix4d &changeOfBaseMatrix)
	{
	O = (Point)O.matrixProduct(changeOfBaseMatrix);
	D = (Vector)D.matrixProduct(changeOfBaseMatrix);
	}*/

	Point at(double t) const
	{
		return O + t*D;
	}

};

class Hit
{
public:
	double t;
	Vector N;
	bool no_hit;

	Hit(const double t, const Vector &normal, bool nohit = false)
		: t(t), N(normal), no_hit(nohit)
	{ }

	static const Hit NO_HIT() { static Hit no_hit(std::numeric_limits<double>::quiet_NaN(), Vector(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()), true); return no_hit; }

};

#endif /* end of include guard: LIGHT_H_PG2BAJRA */
