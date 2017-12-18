#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "triple.h"
#include <Eigen/Dense>

class Camera
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Camera(Triple eye_, Triple center_, Triple up_, int w_, int h_) : eye(eye_), center(center_), width(w_), height(h_)
	{
		double zoom = up_.length();
		eyeNormalDirection = (eye - center).normalized();
		eyeSideDirection = up_.cross(eyeNormalDirection).normalized();
		eyeTopDirection = eyeNormalDirection.cross(eyeSideDirection);

		eyeNormalDirection *= zoom;
		eyeTopDirection *= zoom;



		changeOfBaseMatrix << eyeSideDirection.x, eyeTopDirection.x, eyeNormalDirection.x, 0,
			eyeSideDirection.y, eyeTopDirection.y, eyeNormalDirection.y, 0,
			eyeSideDirection.z, eyeTopDirection.z, eyeNormalDirection.z, 0,
			0, 0, 0, 1;

		std::cout << changeOfBaseMatrix << std::endl;
		center = center.matrixProduct(changeOfBaseMatrix);

		changeOfBaseMatrix(0, 3) = width / 2 - center.x;
		changeOfBaseMatrix(1, 3) = height / 2 - center.y;

		std::cout << "\n" << changeOfBaseMatrix << std::endl;
		eye = (Point)eye.matrixProduct(changeOfBaseMatrix);

		/*eyeNormalDirection.normalize();
		changeOfBaseMatrix(0, 2) = eyeNormalDirection.x;
		changeOfBaseMatrix(1, 2) = eyeNormalDirection.y;
		changeOfBaseMatrix(2, 2) = eyeNormalDirection.z;
		std::cout << "\n" << changeOfBaseMatrix << std::endl;*/
	}

	//Eigen::Matrix4d createChangeOfBaseMatrix();
	const Eigen::Matrix4d ChangeOfBaseMatrix() const;
	const int Width() const;
	const int Height() const;
	const Point Eye() const;
	const int getCenterPointX() const;
	const int getCenterPointY() const;
	Vector eyeNormalDirection;
	Vector eyeTopDirection;
	Vector eyeSideDirection;

private:
	Eigen::Matrix4d changeOfBaseMatrix;
	Point eye;
	Point center;
	int width, height;
};

#endif // !CAMERA_H