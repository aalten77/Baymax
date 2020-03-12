
// Ensure the header file is included only once in multi-file projects
#ifndef SO_BACKGROUND_H
#define SO_BACKGROUND_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>
# include <gsim/stb_image.h>
# include <gsim/gs_shared.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoBackground : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec2>   T; // texture coordinates
	//std::vector<GsColor> C; // colors
	int _numpoints;         // saves the number of points

	gsuint _texid, _texid2;

public:
	SoBackground();
	void init(const GlProgram& prog);
	void build();
	void draw(GsMat& tr, GsMat& pr);
};


#endif // SO_BACKGROUND_H
