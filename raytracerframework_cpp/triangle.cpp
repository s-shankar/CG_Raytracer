#include "triangle.h"
#include "material.h"

Hit Triangle::intersect(const Ray & ray)
{
	double t = -1;
	double DdotN = (ray.D).dot(N);

	if (!DdotN)
		return Hit::NO_HIT();

	/*
	Ray : R = ray.O + t*ray.D
	Plane:     P.N + d = 0 => (ray.O + t*ray.D).N + d =0

	thus :
	t = -(ray.O.N +d)/ray.D.N
	*/
	t = -((ray.O).dot(N) + d) / DdotN;

	if (t <= 0)
		return Hit::NO_HIT();

	Point intersectionPoint = ray.O + t * (ray.D);
	Vector V1 = p2 - p1, V2 = p3-p2, V3 = p1-p3;
	Vector N1 = intersectionPoint - p1, N2 = intersectionPoint - p2, N3 = intersectionPoint - p3;

	// checking if point is inside the triangle
	if ((N.dot(V1.cross(N1)) > 0) && (N.dot(V2.cross(N2)) > 0) && (N.dot(V3.cross(N3)) > 0))
		return Hit(t, N.normalized());
	return Hit::NO_HIT();
}

Intersection Triangle::distanceToPlane(const Plane & plane)
{
	double distance = plane.distance(p1);
	return Intersection(distance, p1*plane.n);
}

void Triangle::changeBase(const Eigen::Matrix4d & changeOfBaseMatrix)
{
}

Color Triangle::getColor(Point p)
{
	return material->color;
}

Triangle::~Triangle()
{
}
