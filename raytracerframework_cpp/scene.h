//
//  Framework for a raytracer
//  File: scene.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"
#include <Eigen/Dense>


enum RenderMode
{
	phong,
	gooch,
	zbuffer,
	normal
};

class Scene
{
private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;
	Camera* camera;
	//Eigen::Matrix4d changeOfBaseMatrix;
	RenderMode renderMode;
	bool shadows = false;
	unsigned int maxRecursionDepth = 0;
	unsigned int super_sampling_factor = 1;
	double b = 0.0, y = 0.0;
	double alpha = 0.0, beta = 0.0;
public:
	Color trace(const Ray &ray, unsigned int depth = 0);
	bool getDistanceIntersection(const Ray &ray, const Ray &shadow, Point lightHit, Object &objet);
	void render(Image &img);
	void addObject(Object *o);
	void addLight(Light *l);

	void setRenderMode(string renderMode_);

	void setShadows(string shadow);
	void setMaxRecursionDepth(string depth);
	void setSuperSampling(string factor);
	void setB(double nb);
	void setY(double ny);
	void setAlpha(double nAlpha);
	void setBeta(double nBeta);
	unsigned int getNumObjects() { return objects.size(); }
	unsigned int getNumLights() { return lights.size(); }

	void setCamera(Camera *c);
	const Point Eye() const;
	const int Width() const;
	const int Height() const;

	void changeObjectsBase();
	//void setBaseMatrix();
	/*void setViewSize(int w, int h);
	void setEye(Triple e);
	void setEyeNormalDirection(Triple e);
	void setEyeTopDirection(Triple e);*/
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
