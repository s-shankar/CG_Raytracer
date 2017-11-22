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

#include "sphere.h"
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
    if (cosAlpha <= 0)
        return Hit::NO_HIT();
    
	double OClength = OC.length();
	double sinBeta = r / OClength; //beta is the maximum angle alpha can have to touch the spere, where the ray is tangent of the sphere

	if(acos(cosAlpha) > asin(sinBeta))
		return Hit::NO_HIT();

	//if we are here, we know the ray intersect the sphere
	
	/****************************************************
	* calcules based on al - Kashi theorem
	*
	*     r²			= |OC|² + t² - 2 |OC| t cosAlpha
	* <=> r² - |OC|²	= t² - 2 |OC| t cosAlpha
	* <=> r² - |OC|²	= t² - 2 |OC| t cosAlpha
	* cosAlpha = t / |OC|
	* <=> r² - |OC|²	= t² - 2 |OC| t * (t / |OC|)
	* <=> r² - |OC|²	= t² (1 - 2)
	* <=> t²			= |OC|² - r²
	*
	* <=> t	= sqrt(|OC|² - r²)
	****************************************************/
    double t = sqrt(OClength * OClength - r*r);

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

	//Plus qu'a ttrouver ce point et on a le vecteur normal
	Point intersectionPoint = Point(0, 0, 0);


    Vector N = (intersectionPoint - position).normalized();

    return Hit(t,N);
}
