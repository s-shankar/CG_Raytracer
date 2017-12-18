#include "box.h"
#include "sphere.h"

Hit Box::intersect(const Ray & ray)
{
	/*
	// directions in 3D
	Vector topDir = Vector(0.0, 1.0, 0.0).normalized();
	Vector rightDir = Vector(1.0, 0.0, 0.0).normalized();
	Vector frontDir = Vector(0.0, 0.0, 1.0).normalized();

	double t = 0.0;
	double D_scalar_dir = 0.0;

	// intersection touching the top left hand point
	Vector OC = (position - ray.O);
	Vector OCn = OC.normalized();

	// vectors from front face
	Vector top_front = vertex[1] - vertex[0];
	Vector left_front = vertex[3] - vertex[0];
	Vector right_front = vertex[2] - vertex[1];
	Vector bottom_front = vertex[3] - vertex[2];


	// if intersects the front face
	// if inside the square, find t
	if (top_front.x >= OCn.x && bottom_front.x <= OCn.x && left_front.x <= OCn.x && right_front.x >= OCn.x)
	{
	if (top_front.y >= OCn.y && bottom_front.y <= OCn.y && left_front.y <= OCn.y && right_front.y >= OCn.y)
	{
	D_scalar_dir = ray.D.dot(topDir);
	if (D_scalar_dir != 0)
	t = (top_front - ray.O).dot(topDir) / D_scalar_dir;
	}
	}

	// intersects the back face

	if (t <= 0)
	return Hit::NO_HIT();
	Vector n = position * t / edge;
	return Hit(t, n);*/

	double halfEdge = edge / 2;
	Hit max_hit(std::numeric_limits<double>::infinity(), Vector());
	Hit sphereHit(Sphere(centerOfCube, sqrt(sqrt(halfEdge*halfEdge * 2) + halfEdge*halfEdge)).intersect(ray));
	if (sphereHit.t > max_hit.t)
	{
		return Hit::NO_HIT();
	}

	Vector n = Vector();
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	for (int i = 0; i < faces.size(); i++)
	{
		Hit hit(faces[i].intersect(ray));

		if (hit.t < max_hit.t)
		{
			Point intersectionPoint = ray.at(hit.t);

			Point p1, p2, p3, p4;

			switch (i)
			{
			case 0:
				p1 = vertex[0];
				p2 = vertex[1];
				p3 = vertex[2];
				p4 = vertex[3];
				break;
			case 1:
				p1 = vertex[0];
				p2 = vertex[1];
				p3 = vertex[4];
				p4 = vertex[5];
				break;
			case 2:
				p1 = vertex[0];
				p2 = vertex[2];
				p3 = vertex[4];
				p4 = vertex[6];
				break;
			case 3:
				p1 = vertex[4];
				p2 = vertex[6];
				p3 = vertex[5];
				p4 = vertex[7];
				break;
			case 4:
				p1 = vertex[2];
				p2 = vertex[3];
				p3 = vertex[6];
				p4 = vertex[7];
				break;
			case 5:
				p1 = vertex[1];
				p2 = vertex[3];
				p3 = vertex[5];
				p4 = vertex[7];
				break;
			}

			double maxDistance = sqrt(2 * edge*edge);

			if ((intersectionPoint - p1).length() < maxDistance &&
				(intersectionPoint - p2).length() < maxDistance &&
				(intersectionPoint - p3).length() < maxDistance &&
				(intersectionPoint - p4).length() < maxDistance)
			{
				if (hit.t < min_hit.t)
				{
					min_hit = hit;
					n = faces[i].n;
				}
			}
		}
	}
	return Hit(min_hit.t, n);
}

Intersection Box::distanceToPlane(const Plane & plane)
{
	Point closestPoint = centerOfCube;
	double tempDistance, minDistance = plane.distance(centerOfCube);

	for (Point p : vertex)
	{
		tempDistance = plane.distance(p);
		if (tempDistance < minDistance)
		{
			closestPoint = p;
			minDistance = tempDistance;
		}
	}
	return Intersection(minDistance, closestPoint);
}

void Box::changeBase(const Eigen::Matrix4d &changeOfBaseMatrix)
{
	centerOfCube = (Point)centerOfCube.matrixProduct(changeOfBaseMatrix);

	for (int i = 0; i < vertex.size(); i++)
	{
		vertex[i] = (Point)vertex[i].matrixProduct(changeOfBaseMatrix);
	}

	for (int i = 0; i < faces.size(); i++)
	{
		faces[i].changeBase(changeOfBaseMatrix);
	}
}