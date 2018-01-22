#include <string.h>
#include <vector>
#include "mesh.h"
#include "material.h"
#include "glm.h"


Mesh::Mesh(float s, Point pos, std::string pathname) : scale(s), position(pos), filename(pathname)
{
	GLMmodel* model = glmReadOBJ((char*)filename.c_str());
	std::cout << "Model : " << model->pathname << std::endl << "mtl: " << model->mtllibname<<std::endl<<"vertices : "<< model->numvertices<<std::endl<<"Triangles : "<<model->numtriangles<<"materials : "<<model->nummaterials<<std::endl<<"position: "<<model->position[0]<<" , "<< model->position[1]<<" , "<< model->position[2]<<std::endl;
	glmScale(model, scale);

	// convert model into triangles for scene
	GLMgroup *group = model->groups;
	int nbTriangle = model->numtriangles;
	while (group)
	{
		for (int i = 0; i < nbTriangle;++i)
		{
			GLMtriangle triangle = model->triangles[group->triangles[i]];
			
			// points of the triangle
			Point p1(model->vertices[triangle.vindices[0] * 3], model->vertices[triangle.vindices[0] * 3 + 1], model->vertices[triangle.vindices[0] * 3 + 2]);
			Point p2(model->vertices[triangle.vindices[1] * 3], model->vertices[triangle.vindices[1] * 3 + 1], model->vertices[triangle.vindices[1] * 3 + 2]);
			Point p3(model->vertices[triangle.vindices[2] * 3], model->vertices[triangle.vindices[2] * 3 + 1], model->vertices[triangle.vindices[2] * 3 + 2]);

			p1 += position;
			p2 += position;
			p3 += position;

			Triangle* tr = new Triangle(p1, p2, p3);

			// if there is any texture or defined color for the face, get it , 
			// otherwise use the one from the yaml file

			std::string mtlGroupName = std::string(group->name);
			int nMaterial = glmFindMaterial(model, (char*)mtlGroupName.c_str());
			if (nMaterial)
			{
				GLMmaterial m = model->materials[nMaterial];
				Material *material = new Material();
				material->ka = m.ambient[0];
				material->kd = m.diffuse[0];
				material->ks = m.specular[0];
				material->color = Color(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
				material->n = m.shininess;
				tr->material = material;
			}
			triangles.push_back(tr);
		}
		group = group->next;
	}
	glmDelete(model);
}

Hit Mesh::intersect(const Ray & ray)
{
	return Hit::NO_HIT();
}

Intersection Mesh::distanceToPlane(const Plane & plane)
{
	return Intersection();
}

void Mesh::changeBase(const Eigen::Matrix4d & changeOfBaseMatrix)
{
}

Color Mesh::getColor(Point p)
{
	return Color();
}

Mesh::~Mesh()
{
}
