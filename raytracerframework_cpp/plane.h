#ifndef PLANE_H
#define PLANE_H

#include "object.h"

class Plane : public Object
{
public:
	Plane(Point p_, Vector n_) : p(p_), n(n_) {}

	virtual Hit intersect(const Ray &ray);
	virtual Intersection distanceToPlane(const Plane &plane);
	virtual void changeBase(const Eigen::Matrix4d &changeOfBaseMatrix);
	virtual Color getColor(Point p);

	double operator-(const Plane &plane);
	double distance(const Point &point) const;

	/*const*/ Point p;
	/*const*/ Vector n;
};

class Intersection
{
public:
	Intersection() : t(0), p(Point(0, 0, 0)) {}
	Intersection(double t_, Point p_) : t(t_), p(p_) {}

	double t;
	Point p;
};

#endif // !BOX_H