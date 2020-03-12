#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "Light.h"


// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:

class SoCapsule : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec> N; //normals
	std::vector<GsColor> C; // colors
	int * _numpoints;         // saves the number of points
	GsMat transformationTop;
	GsMat transformationBottom;

public:
	SoCapsule();
	void init(const GlProgram& prog);
	void build(float len, float rt, float rb);
	void draw(GsMat& tr, GsMat& pr, const Light& l, float colorMult);
};

#endif // SO_CAPSULE_H
