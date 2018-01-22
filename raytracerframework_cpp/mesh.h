#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include <string.h>
#include <vector>

class Mesh : public Object
{
public:
	Point position;
	float scale;
	std::string filename;
	std::vector<Triangle*> triangles;
	Mesh(float s, Point pos, std::string pathname);
	virtual Hit intersect(const Ray &ray);
	virtual Intersection distanceToPlane(const Plane &plane);
	void Mesh::changeBase(const Eigen::Matrix4d &changeOfBaseMatrix);
	virtual Color getColor(Point p);
	~Mesh();
};

#endif // !MESH_H

