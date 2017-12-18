// Students : Tristan HERNANT - Shankar SIVAGNA
//
//  Framework for a raytracer
//  File: scene.cpp
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

#include <iostream>
#include <string>
#include "scene.h"
#include "material.h"
#include "plane.h"

Color Scene::trace(const Ray &ray, unsigned int depth)
{
	// Find hit object and distance
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit(objects[i]->intersect(ray));
		if (hit.t<min_hit.t) {
			min_hit = hit;
			obj = objects[i];
		}
	}

	// No hit? Return background color.
	if (!obj) return Color(0.0, 0.0, 0.0);

	Material *material = obj->material;            //the hit objects material
	Point hit = ray.at(min_hit.t);                 //the hit point
	Vector N = min_hit.N;                          //the normal at hit point
	Vector V = -ray.D;                             //the view vector


												   /****************************************************
												   * This is where you should insert the color
												   * calculation (Phong model).l
												   *
												   * Given: material, hit, N, V, lights[]
												   * Sought: color
												   *
												   * Hints: (see triple.h)
												   *        Triple.dot(Vector) dot product
												   *        Vector+Vector      vector sum
												   *        Vector-Vector      vector difference
												   *        Point-Point        yields vector
												   *        Vector.normalize() normalizes vector, returns length
												   *        double*Color        scales each color component (r,g,b)
												   *        Color*Color        dito
												   *        pow(a,b)           a to the power of b
												   ****************************************************/

	if (renderMode == RenderMode::phong)
	{
		/* Using Phong Shading formula */
		Vector L, R;

		Color color;

		//ambiant member
		double ambiant = 0;

		Color diffuse, specular;
		bool shdw;

		/*	For each Light Point, check if the Light Source
		*	hits the Object.
		*	Comparing with L, V, R and N vectors. Need to normalizes vectors, so that
		*	their dot product gives the cosinus of their angle.
		*	It will enable to compute the illumination thanks to
		*	the colour the Light Point and the Object colour.
		*
		*	Assuming the intensity properties of the Object are all equal to 1.
		*/

		for (size_t i = 0; i < lights.size(); i++)
		{
			/* Light vector given by the distance between
			the Light Point and Object hit Point. */
			L = (lights.at(i)->position - hit).normalized();

			shdw = false;
			if (shadows)
			{
				/* get shadow ray and see if it intersects another object for shadow*/
				//Ray shadowRay(hit, L);
				shdw = getDistanceIntersection(ray, Ray(hit, L), hit, *obj);
			}

			ambiant = material->ka;

			if (!shdw)
			{
				double ldotn = L.dot(N);
				// diffusion  : (L.N)
				if (ldotn >= 0)
					diffuse += ldotn * lights.at(i)->color;

				// R = 2(N.L)N - L
				R = (2 * ldotn * N - L).normalized();

				// specular : (R.V)^alpha
				double rdotv = R.dot(V);
				if (rdotv >= 0)
					specular += pow(rdotv, material->n) * lights.at(i)->color;
			}

			color = (ambiant + diffuse*material->kd)*material->color + specular*material->ks;
		}

		if (depth < maxRecursionDepth)
		{
			/*
			*  Reflection ray dir = Incident ray dir - 2 * (incident ray dir dot surface normal) * surface normal
			* 	R = I - 2*cos(theta)*N
			*/
			Vector reflectionVector(ray.D - (2 * (ray.D.dot(N))*N));
			Ray reflectionRay(hit, reflectionVector);
			// Call recursively in order to get correct reflection color
			Color reflectionColor = trace(reflectionRay, depth + 1);
			color += reflectionColor*material->ks;
		}

		return color;
	}

	if (renderMode == RenderMode::zbuffer)
	{
		Plane	farClippingPlane(Point(0, 0, 0), camera->eyeNormalDirection),
			maxNearClippingPlane(camera->Eye(), camera->eyeNormalDirection);

		Point p = camera->Eye();
		double maxDistance = maxNearClippingPlane - farClippingPlane, d = maxDistance;
		Intersection intersection;
		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			intersection = objects[i]->distanceToPlane(maxNearClippingPlane);
			if (intersection.t < d)
			{
				d = intersection.t;
				p = intersection.p;
			}
		}

		Plane nearClippingPlane(p, camera->eyeNormalDirection);
		maxDistance = nearClippingPlane - farClippingPlane;

		double	distance = min_hit.t - (maxNearClippingPlane - nearClippingPlane);

		if (distance < 0)
			distance = 0;
		else if (maxDistance - distance < 0)
			distance = maxDistance;


		Color color(0, 0, 0);

		color += 1 - (distance / maxDistance);

		return color;
	}

	if (renderMode == RenderMode::normal)
	{
		return (N + Vector(1, 1, 1)) / 2;
	}

	return  material->color;
}

bool Scene::getDistanceIntersection(const Ray & ray, const Ray &shadow, Point lightHit, Object &objet)
{
	/* find intersection between another object and shadowRay and compute according to distance */
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		if (objects[i] == obj)
			continue;
		Hit hit(objects[i]->intersect(shadow));

		// face culling in order to reject bad intersections
		if (-ray.D.dot(hit.N) <0 && hit.t<min_hit.t)
		{
			min_hit = hit;
			obj = objects[i];
		}
	}

	if (obj)
	{
		Point s_hit = shadow.at(min_hit.t);
		bool distance = (lightHit - ray.at(min_hit.t)).length() < (lightHit - ray.D).length();
		// correct distance enable shadowing
		if (!distance)
			return true;
	}
	return false;
}

void Scene::render(Image &img)
{
	double centerX = camera->getCenterPointX();
	double centerY = camera->getCenterPointY();
	int w = img.width();
	int h = img.height();

	double factor = (static_cast<double>(super_sampling_factor) * static_cast<double>(super_sampling_factor)) / 1.0;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			/*Point pixel(x + 0.5, h - 1 - y + 0.5, 0);
			Ray ray(camera->Eye(), (pixel - camera->Eye()).normalized());
			Color col = trace(ray);
			col.clamp();
			img(x, y) = col;*/

			// store list of colors for the pixel
			std::vector<Color> sampleColorList;
			double offset = 1.0 / (super_sampling_factor + 1);
			for (unsigned int i = 0; i < super_sampling_factor; i++)
			{
				for (unsigned int j = 0; j < super_sampling_factor; j++)
				{
					Point pixel(x + i * offset, h - 1 - y + j * offset, 0);
					Ray ray(Eye(), (pixel - Eye()).normalized());
					Color col = trace(ray);
					col.clamp();
					sampleColorList.push_back(col);
				}
			}

			Color ultimateColor(sampleColorList[0]);
			for (unsigned int i = 1; i < sampleColorList.size(); i++)
				ultimateColor += sampleColorList[i];

			ultimateColor /= factor;
			ultimateColor.clamp();
			img(x, y) = ultimateColor;
		}
	}
}

void Scene::addObject(Object *o)
{
	objects.push_back(o);
}

void Scene::addLight(Light *l)
{
	lights.push_back(l);
}


void Scene::setCamera(Camera *c)
{
	camera = c;
}

/*
void Scene::setBaseMatrix()
{
changeOfBaseMatrix = camera->createChangeOfBaseMatrix();
}*/
/*
void Scene::setEye(Triple e)
{
eye = e;
}

void Scene::setEyeNormalDirection(Triple e)
{
eyeNormalDirection = e;
}

void Scene::setEyeTopDirection(Triple e)
{
eyeTopDirection = e;
}
*/


void Scene::setRenderMode(string renderMode_)
{
	if (renderMode_ == "phong")
		renderMode = phong;

	if (renderMode_ == "zbuffer")
		renderMode = zbuffer;

	if (renderMode_ == "normal")
		renderMode = normal;
}

void Scene::setShadows(string shadow)
{
	if (shadow == "true")
		shadows = true;
	else if (shadow == "false")
		shadows = false;
}

void Scene::setMaxRecursionDepth(string depth)
{
	maxRecursionDepth = std::stoi(depth);
}
/*
void Scene::setViewSize(int w, int h)
{
width = w;
height = h;
}*/

const int Scene::Width() const
{
	return camera->Width();
}

const int Scene::Height() const
{
	return camera->Height();
}

const Point Scene::Eye() const
{
	return camera->Eye();
}

void Scene::setSuperSampling(string factor)
{
	size_t f = std::stoi(factor);
	if (f>0)
		super_sampling_factor = f;
}

void Scene::changeObjectsBase()
{
	Eigen::Matrix4d matrix = camera->ChangeOfBaseMatrix();

	for (Object* o : objects)
	{
		o->changeBase(matrix);
	}

	for (Light* l : lights)
	{
		l->changeBase(matrix);
	}
}