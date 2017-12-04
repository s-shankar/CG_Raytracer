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
	Point center = position /*- Point(edge/2.0,edge/2.0,0)*/;
	Vector OC = (center - ray.O);
	Vector OCn = OC.normalized();

	// points of front face
	Point top_left_front = vertex[0];
	Point top_right_front = vertex[1];
	Point bottom_right_front = vertex[2];
	Point bottom_left_front = vertex[3];

	// poins from back face
	Point top_left_back = vertex[7];
	Point top_right_back = vertex[6];
	Point bottom_right_back = vertex[5];
	Point bottom_left_back = vertex[4];

	// vectors from front face
	Vector top_front = vertex[1] - vertex[0];
	Vector left_front = vertex[3] - vertex[0];
	Vector right_front = vertex[2] - vertex[1];
	Vector bottom_front = vertex[3] - vertex[2];

	// back face
	Vector top_back = top_front - Point(0,0,edge);
	Vector left_back = left_front - Point(0, 0, edge);
	Vector right_back = right_back - Point(0, 0, edge);
	Vector bottom_back = bottom_front - Point(0, 0, edge);

	// if intersects the front face
	// if inside the square, find t
	if (top_left_front.x <= OC.x && bottom_left_front.x <= OC.x && top_right_front.x >= OC.x && bottom_right_front.x >= OC.x)
	{
		if (top_left_front.y >= OC.y && top_right_front.y >= OC.y && bottom_right_front.y <= OC.y && bottom_left_front.y <= OC.y)
		{
			D_scalar_dir = ray.D.dot(frontDir);
			if(D_scalar_dir != 0)
				t = (top_front - ray.O).dot(-frontDir) / D_scalar_dir;
		}
	}

	// back face
	if (top_left_back.x <= OC.x && bottom_left_back.x <= OC.x && top_right_back.x >= OC.x && bottom_right_back.x >= OC.x)
	{
		if (top_left_back.y >= OC.y && top_right_back.y >= OC.y && bottom_right_back.y <= OC.y && bottom_left_back.y <= OC.y)
		{
			D_scalar_dir = ray.D.dot(-frontDir);
			if (D_scalar_dir != 0)
				t = (top_front - ray.O).dot(frontDir) / D_scalar_dir;
		}
	}


	// top face
	if (top_left_front.y <= OC.y && top_left_back.y <= OC.y && top_right_front.y >= OC.y && top_right_back.y >= OC.y)
	{
		if (top_left_front.z >= OC.z && top_left_back.z >= OC.z && top_right_front.z <= OC.z && top_right_back.z <= OC.z)
		{
			D_scalar_dir = ray.D.dot(topDir);
			if (D_scalar_dir != 0)
				t = (top_front - ray.O).dot(topDir) / D_scalar_dir;
		}
	}

	// bottom face
	if (bottom_left_front.z <= OC.z && bottom_right_front.z <= OC.z && bottom_right_back.z >= OC.z && bottom_left_back.z >= OC.z)
	{
		if (bottom_right_front.x >= OC.x && bottom_right_back.x >= OC.x && bottom_left_front.x <= OC.x && bottom_left_back.x <= OC.x)
		{
			D_scalar_dir = ray.D.dot(topDir);
			if (D_scalar_dir != 0)
				t = (top_front - ray.O).dot(topDir) / D_scalar_dir;
		}
	}

	// left side face
	if (top_left_back.y <= OC.y && bottom_left_back.y <= OC.y && top_right_back.y >= OC.y && bottom_right_back.y >= OC.y)
	{
		if (top_left_back.z >= OC.z && top_right_back.z >= OC.z && bottom_right_back.z <= OC.z && bottom_left_back.z <= OC.z)
		{
			D_scalar_dir = ray.D.dot(-rightDir);
			if (D_scalar_dir != 0)
				t = (top_front - ray.O).dot(rightDir) / D_scalar_dir;
		}
	}

	// right side face
	if (top_right_front.z <= OC.y && bottom_right_back.z <= OC.z && top_right_back.z >= OC.z && bottom_right_back.z >= OC.z)
	{
		if (top_right_back.y >= OC.y && top_right_front.y >= OC.y && bottom_right_back.y <= OC.y && bottom_right_front.y <= OC.y)
		{
			D_scalar_dir = ray.D.dot(rightDir);
			if (D_scalar_dir != 0)
				t = (top_front - ray.O).dot(-rightDir) / D_scalar_dir;
		}
	}

	if(t<=0)
		return Hit::NO_HIT();
	Vector n = position * t/ edge;
	return Hit(t, n);
}
