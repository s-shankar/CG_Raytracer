// Students : Tristan HERNANT - Shankar SIVAGNA
//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "box.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);
string parseString(const YAML::Node& node);
int parseWidth(const YAML::Node& node);
int parseHeight(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
	assert(node.size() == 3);
	node[0] >> t.x;
	node[1] >> t.y;
	node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
	Triple t;
	node[0] >> t.x;
	node[1] >> t.y;
	node[2] >> t.z;
	return t;
}

string parseString(const YAML::Node& node)
{
	string res;
	node >> res;
	return res;
}

int parseWidth(const YAML::Node& node)
{
	int res;
	node[0] >> res;
	return res;
}

int parseHeight(const YAML::Node& node)
{
	int res;
	node[1] >> res;
	return res;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
	Material *m = new Material();
	if (node.FindValue("texture") != 0)
	{
		std::string texturePath;
		node["texture"] >> texturePath;
		std::cout << "Texture detected : " << texturePath << std::endl;
		m->texture = new Image(texturePath.c_str());
	}
	else
	{
		node["color"] >> m->color;
	}
	node["ka"] >> m->ka;
	node["kd"] >> m->kd;
	node["ks"] >> m->ks;
	node["n"] >> m->n;
	return m;
}

Camera* Raytracer::parseCamera(const YAML::Node& node)
{

	Camera *c = new Camera(parseTriple(node["eye"]), parseTriple(node["center"]), parseTriple(node["up"]), parseWidth(node["viewSize"]), parseHeight(node["viewSize"]));
	return c;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
	Object *returnObject = NULL;
	std::string objectType;
	node["type"] >> objectType;

	if (objectType == "sphere") {
		Point pos;
		node["position"] >> pos;
		double r;
		node["radius"] >> r;
		Sphere *sphere = new Sphere(pos, r);
		returnObject = sphere;
	}

	if (objectType == "plane") {
		Point p;
		node["point"] >> p;
		Vector n;
		node["normal"] >> n;
		Plane *plane = new Plane(p, n.normalized());
		returnObject = plane;
	}

	if (objectType == "box")
	{
		Point pos;
		node["centerOfCube"] >> pos;
		double e;
		node["edge"] >> e;
		Vector normalUpFace;
		node["normalUpFace"] >> normalUpFace;
		Vector normalSideFace;
		node["normalSideFace"] >> normalSideFace;
		Box *box = new Box(pos, e, normalUpFace, normalSideFace);
		returnObject = box;
	}

	if (returnObject) {
		// read the material and attach to object
		returnObject->material = parseMaterial(node["material"]);
		if (node.FindValue("rotation") != 0)
		{
			returnObject->setRotate(node["rotation"][0], node["rotation"][1],node["rotation"][2]);
		}
	}

	return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
	Point position;
	node["position"] >> position;
	Color color;
	node["color"] >> color;
	return new Light(position, color);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
	// Initialize a new scene
	scene = new Scene();

	// Open file stream for reading and have the YAML module parse it
	std::ifstream fin(inputFilename.c_str());
	if (!fin) {
		cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
		return false;
	}
	try {
		YAML::Parser parser(fin);
		if (parser) {
			YAML::Node doc;
			parser.GetNextDocument(doc);

			// Read render mode
			scene->setRenderMode(parseString(doc["RenderMode"]));
			//scene->setViewSize(parseInt(doc["Width"]), parseInt(doc["Height"]));

			// if any gooch parameters
			if (doc.FindValue("GoochParameters"))
			{
				scene->setB(doc["GoochParameters"]["b"]);
				scene->setY(doc["GoochParameters"]["y"]);
				scene->setAlpha(doc["GoochParameters"]["alpha"]);
				scene->setBeta(doc["GoochParameters"]["beta"]);
			}

			// Read if shadow mode is enabled
			if(doc.FindValue("Shadows") !=0)
				scene->setShadows(parseString(doc["Shadows"]));

			// Read if there is any any maxRecursiveDepth in file
			if (doc.FindValue("MaxRecursionDepth") != 0)
				scene->setMaxRecursionDepth(parseString(doc["MaxRecursionDepth"]));

			// Enable super sampling or not
			if (doc.FindValue("SuperSampling") != 0)
				scene->setSuperSampling(doc["SuperSampling"]["factor"]);

			// Read scene configuration options

			scene->setCamera(parseCamera(doc["Camera"]));
			//scene->setBaseMatrix();
			/*scene->setEye(parseTriple(doc["Eye"]));
			scene->setEyeNormalDirection(parseTriple(doc["EyeNormalDirection"]));
			scene->setEyeTopDirection(parseTriple(doc["EyeTopDirection"]));
			scene->createChangeOfBaseMatrix(); */

			// Read and parse the scene objects
			const YAML::Node& sceneObjects = doc["Objects"];
			if (sceneObjects.GetType() != YAML::CT_SEQUENCE)
			{
				cerr << "Error: expected a sequence of objects." << endl;
				return false;
			}
			for (YAML::Iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
				Object *obj = parseObject(*it);
				// Only add object if it is recognized
				if (obj) {
					scene->addObject(obj);
				}
				else {
					cerr << "Warning: found object of unknown type, ignored." << endl;
				}
			}

			// Read and parse light definitions
			const YAML::Node& sceneLights = doc["Lights"];
			if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
				cerr << "Error: expected a sequence of lights." << endl;
				return false;
			}
			for (YAML::Iterator it = sceneLights.begin(); it != sceneLights.end(); ++it) {
				scene->addLight(parseLight(*it));
			}
		}
		if (parser) {
			cerr << "Warning: unexpected YAML document, ignored." << endl;
		}
	}
	catch (YAML::ParserException& e) {
		std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
		return false;
	}

	cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
	return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
	scene->changeObjectsBase();
	Image img(scene->Width(), scene->Height());
	cout << "Tracing..." << endl;
	scene->render(img);
	cout << "Writing image to " << outputFilename << "..." << endl;
	img.write_png(outputFilename.c_str());
	cout << "Done." << endl;
}
