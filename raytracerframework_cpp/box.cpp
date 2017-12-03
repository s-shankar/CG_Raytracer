#include "box.h"

Hit Box::intersect(const Ray & ray)
{
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
			if(D_scalar_dir != 0)
				t = (top_front - ray.O).dot(topDir) / D_scalar_dir;
		}
	}

	// intersects the back face
	
	if(t<=0)
		return Hit::NO_HIT();
	Vector n = position * t/ edge;
	return Hit(t, n);
}
