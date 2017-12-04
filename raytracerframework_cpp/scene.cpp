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

#include "scene.h"
#include "material.h"
#include "plane.h"

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
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

	if(renderMode == RenderMode::phong)
	{
		/* Using Phong Shading formula */
		Vector L, R;

		//ambiant member
		double ambiant = material->ka;                  // place holder

														//difuse and specular member
		Color diffuse, specular;

		/* For each Light Point, check if the Light Source
		hits the Object.
		Comparing with L, V, R and N vectors. Need to normalizes vectors, so that
		their dot product gives the cosinus of their angle.
		It will enable to compute the illumination thanks to
		the colour the Light Point and the Object colour.

		Assuming the intensity properties of the Object are all equal to 1.*/
		for (size_t i = 0; i < lights.size(); i++)
		{
			/* Light vector given by the distance between
			the Light Point and Object hit Point. */
			L = (lights.at(i)->position - hit).normalized();

			// diffusion  : (L.N)
			if (L.dot(N) > 0)
				diffuse += L.dot(N) * lights.at(i)->color;

			// R = 2(N.L)N - L
			R = (2 * L.dot(N) * N - L).normalized();

			// specular : (R.V)^alpha
			if (R.dot(V) > 0)
				specular += pow(R.dot(V), material->n) * lights.at(i)->color;
		}
		diffuse *= material->kd;
		specular *= material->ks;

		return (ambiant + diffuse) * material->color + specular;
	}

	if (renderMode == RenderMode::zbuffer)
	{
		Plane	farClippingPlane(Point(0, 0, 0), normalNearClippingPlane),
				maxNearClippingPlane(eye, normalNearClippingPlane);

		Point p = eye;
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

		Plane nearClippingPlane(p, normalNearClippingPlane);
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

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x,y) = col;
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

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setNormalNearClippingPlane(Triple e)
{
	normalNearClippingPlane = e;
}

void Scene::setRenderMode(string renderMode_)
{
	if(renderMode_ == "phong")
		renderMode = phong;

	if (renderMode_ == "zbuffer")
		renderMode = zbuffer;

	if (renderMode_ == "normal")
		renderMode = normal;
}