#include <iostream>
#include "camera.h"

/*
Eigen::Matrix4d Camera::createChangeOfBaseMatrix()
{
Eigen::Matrix4d res;

res << eyeSideDirection.x, eyeTopDirection.x, eyeNormalDirection.x,
eyeSideDirection.y, eyeTopDirection.y, eyeNormalDirection.y,
eyeSideDirection.z, eyeTopDirection.z, eyeNormalDirection.z;

return res;
}*/

const Eigen::Matrix4d Camera::ChangeOfBaseMatrix() const
{
	return changeOfBaseMatrix;
}

const int Camera::Width() const
{
	return width;
}

const int Camera::Height() const
{
	return height;
}

const Point Camera::Eye() const
{
	return eye;
}

const int Camera::getCenterPointX() const
{
	return center.x;
}

const int Camera::getCenterPointY() const
{
	return center.y;
}