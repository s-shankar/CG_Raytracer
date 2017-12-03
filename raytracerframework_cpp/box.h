#ifndef BOX_H
#define BOX_H

#include "object.h"

class Box : public Object
{
public:
	Box(Point v, double s) : vertex(v), edge(s) {}

	virtual Hit intersect(const Ray &ray);
	/*
	Need 8 points and 12 edges
	*/

	/* first vertex, then we can determine where the 7 others are
		should we also consider direction of the box ?
	*/
	const Point vertex;
	const double edge;
};

#endif // !BOX_H
