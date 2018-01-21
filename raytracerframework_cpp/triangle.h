#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "plane.h"

class Triangle : public Object
{
public :
	const Point p1, p2, p3; // vertices
	Vector N; // normal vector of the triangle

	/* let A(xa, ya, za) be a point that belongs to the plan of the triangle.
		According to the cartesian equation of a plan with a point M(x,y,z) & the normal vector N(a,b,c):
		ax + by + cz +d = 0
		d =  -a*xa - b*ya - c*za 
		we will take Point1 as A in order to get d */
	double d = 0.0;

	Triangle(Point point1, Point point2, Point point3) : p1(point1), p2(point2), p3(point3)
	{
		Vector v = p2 - p1; 
		N = v.cross(p3 - p1);
		d = -(N.x)*p1.x - N.y*p1.y - N.z*p1.z;
	}
	virtual Hit intersect(const Ray &ray);
	virtual Intersection distanceToPlane(const Plane &plane);
	void Triangle::changeBase(const Eigen::Matrix4d &changeOfBaseMatrix);
	virtual Color getColor(Point p);
};
#endif // TRIANGLE_H