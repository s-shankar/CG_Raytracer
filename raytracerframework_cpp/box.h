#ifndef BOX_H
#define BOX_H

#include <iostream>
#include <vector>
#include "plane.h"

class Box : public Object
{
public:
	Box(Point v, double s) : position(v), edge(s)
	{
		// get 4 points for the front face

		Point p1 = position; // top left corner
		Point p2 = position + (Point(edge, 0, 0)); // top right 
		Point p3 = p2 - (Point(0, edge, 0)); // bottom right
		Point p4 = p3 - (Point(edge, 0, 0)); // bottom left

		Point p5 = p4 - (Point(0, 0, edge)); // bottom left behind
		Point p6 = p5 + (Point(edge, 0, 0)); // bottom right behind
		Point p7 = p6 + (Point(0, edge, 0)); // top right behind
		Point p8 = p7 - (Point(edge, 0, 0)); // top left behind

		vertex.push_back(p1);
		vertex.push_back(p2);
		vertex.push_back(p3);
		vertex.push_back(p4);
		vertex.push_back(p5);
		vertex.push_back(p6);
		vertex.push_back(p7);
		vertex.push_back(p8);
	}

	virtual Hit intersect(const Ray &ray);
	virtual Intersection distanceToPlane(const Plane &plane);
	/*
	Need 8 points and 12 edges
	*/

	/* first vertex, then we can determine where the 7 others are
	should we also consider direction of the box ?
	*/
	const Point position;
	std::vector<Point> vertex;
	const double edge;
};

#endif // !BOX_H
