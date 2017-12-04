#include "plane.h"
#include <iostream>
#include <math.h>

Hit Plane::intersect(const Ray &ray)
{
	double nDotD = n.dot(ray.D); 

	if (nDotD <= 0)
		return Hit::NO_HIT();


	double t = 0;

	if (nDotD == 1)
	{
		t = (ray.O - p).dot(n); //normally the absolute value, but nDotD == 1 so here (ray.O - p).dot(n) is positive
	}
	else
	{
		t = (p - ray.O).dot(n) / ray.D.dot(n);
	}


	/****************************************************
	* RT1.2: NORMAL CALCULATION
	*
	* Given: t, C, r
	* Sought: N
	*
	* Insert calculation of the sphere's normal at the intersection point.
	****************************************************/

	return Hit(t, n);
}

double Plane::operator-(const Plane &plane)
{
	if(abs(n.dot(plane.n)) != 1)
		return 0;

	return (p - plane.p).dot(n) / plane.n.dot(n);
}

Intersection Plane::distanceToPlane(const Plane &plane)
{
	if (abs(n.dot(plane.n)) != 1)
		return Intersection(0, plane.p);

	return  Intersection((p - plane.p).dot(n) / plane.n.dot(n), plane.p);
}

double Plane::distance(const Point &point) const
{
	return abs(point.dot(n) - p.dot(n))/ n.length();
}