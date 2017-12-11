#ifndef BOX_H
#define BOX_H

#include <iostream>
#include <vector>
#include "plane.h"

class Box : public Object
{
public:
	Box(Point v, double s, Vector normalUpFace, Vector normalSideFace) : centerOfCube(v), edge(s)
	{
		normalUpFace.normalize();
		normalSideFace.normalize();

		Vector lastNormal = normalUpFace.cross(normalSideFace);

		double halfEdge = edge / 2;

		faces.push_back(Plane(centerOfCube + normalUpFace * halfEdge, normalUpFace));
		faces.push_back(Plane(centerOfCube + normalSideFace * halfEdge, normalSideFace));
		faces.push_back(Plane(centerOfCube + lastNormal * halfEdge, lastNormal));
		faces.push_back(Plane(centerOfCube + (-1) * normalUpFace * halfEdge, (-1) * normalUpFace));
		faces.push_back(Plane(centerOfCube + (-1) * normalSideFace * halfEdge, (-1) * normalSideFace));
		faces.push_back(Plane(centerOfCube + (-1) * lastNormal * halfEdge, (-1) * lastNormal));

		

		Point p1 = centerOfCube + (halfEdge * faces[0].n + halfEdge * faces[1].n + halfEdge * faces[2].n);
		Point p2 = p1 + (edge * faces[5].n);
		Point p3 = p1 + (edge * faces[4].n);
		Point p4 = p1 + (edge * faces[4].n + edge * faces[5].n);

		Point p5 = centerOfCube + (halfEdge * faces[3].n + halfEdge * faces[1].n + halfEdge * faces[2].n); // bottom left behind
		Point p6 = p5 + (edge * faces[5].n); // bottom right behind
		Point p7 = p5 + (edge * faces[4].n);  // top right behind
		Point p8 = p5 + (edge * faces[4].n + edge * faces[5].n); // top left behind

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
	void changeBase(const Eigen::Matrix3d &changeOfBaseMatrix);

	Point centerOfCube;
	std::vector<Point> vertex;
	std::vector<Plane> faces;
	const double edge;
};

#endif // !BOX_H
