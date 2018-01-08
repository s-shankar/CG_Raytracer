// Students : Tristan HERNANT - Shankar SIVAGNA
//
//  Framework for a raytracer
//  File: sphere.cpp
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
#define _USE_MATH_DEFINES

#include "sphere.h"
#include "material.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	*
	* Given: ray, position, r
	* Sought: intersects? if true: *t
	*
	* Insert calculation of ray/sphere intersection here.
	*
	* You have the sphere's center (C) and radius (r) as well as
	* the ray's origin (ray.O) and direction (ray.D).
	*
	* If the ray does not intersect the sphere, return Hit::NO_HIT().
	* Otherwise, return an instance of Hit() with the distance of the
	* intersection point from the ray origin as t and the normal ad N (see example).
	****************************************************/

	// place holder for actual intersection calculation

	Vector OC = (position - ray.O);
	Vector OCn = OC.normalized();
	double cosAlpha = OCn.dot(ray.D); //here alpha is the angle formed by the vectors CO and D

									  /*case where the ray is directed behind the direction of the sphere*/
	if (acos(cosAlpha) < 0)
		return Hit::NO_HIT();

	double OClength = OC.length();
	double sinBeta = r / OClength; //beta is the maximum angle alpha can have to touch the sphere, where the ray is tangent to the sphere

	if (acos(cosAlpha) > asin(sinBeta))
		return Hit::NO_HIT();

	//if we are here, we know the ray intersects the sphere

	/****************************************************
	* calculations based on al-Kashi theorem
	*
	*     r²			= |OC|² + t² - 2 |OC| t cosAlpha
	* <=> r² - |OC|²	= t² - 2 |OC| t cosAlpha
	* <=> r² - |OC|²	= t² - 2 |OC| t cosAlpha

	* here, we have an equation of the form : a x² + b x + c = 0
	* we calculate the discriminant delta = b² - 4 a c
	* if delta is negative, the ray does not intersect the sphere which should not be happening
	* else, there is one solution when the ray intersects but is tangent of the sphere (delta == 0)
	* or two solutions when delta is positive, in which case we only accept the one which represents the first intersection of the ray and the sphere
	****************************************************/
	double t, b = -2 * OClength*cosAlpha;

	double delta = b*b + 4 * (r*r - OClength*OClength);

	//just in case
	if (delta < 0)
		return Hit::NO_HIT();

	t = (-b - sqrt(delta)) / 2;
	double t2 = (-b + sqrt(delta)) / 2;

	if (t2 < t)
		t = t2;

	if (t < 0.0)
		return Hit::NO_HIT();
	/****************************************************
	* RT1.2: NORMAL CALCULATION
	*
	* Given: t, C, r
	* Sought: N
	*
	* Insert calculation of the sphere's normal at the intersection point.
	****************************************************/


	Point intersectionPoint = ray.at(t);
	Vector N = -1 * (position - intersectionPoint) / r;

	return Hit(t, N);
}

Intersection Sphere::distanceToPlane(const Plane &plane)
{
	double distance = plane.distance(position) - r;
	return Intersection(distance, position + r * plane.n);
}

void Sphere::changeBase(const Eigen::Matrix4d &changeOfBaseMatrix)
{
	position = (Point)position.matrixProduct(changeOfBaseMatrix);
}

Color Sphere::getColor(Point p)
{
	if (this->material->texture == NULL)
		return material->color;
	
	// get point into spherical coordinates
	Vector translation = p-position;
	double rho=0.0, nTheta=0.0, nPhi=0.0;
	rho = translation.length();
	nTheta = atan2(translation.y, translation.x) + theta;
	nPhi = acos(translation.z / rho);

	// getting the spherical coordonates
	double x_rotate = 0.0, y_rotate = 0.0, z_rotate = 0.0;
	x_rotate = rho * cos(nTheta)*cos(nPhi);
	y_rotate = rho * sin(nTheta)*cos(nPhi);
	z_rotate = rho * sin(nPhi);

	Vector rotateVector(x_rotate, y_rotate, z_rotate);
	rotateVector += position;
	Vector normalizedRotateVector = (this->position - rotateVector).normalized();
	// sphere texture mapping using u-v, bounded by [0,1] & [0,1]
	double u, v;
	u = 0.5 + atan2(normalizedRotateVector.x,normalizedRotateVector.y)/(2*M_PI);
	v = 0.5 + asin(normalizedRotateVector.z) / M_PI;
	
	return this->material->texture->colorAt(u,v);
}
