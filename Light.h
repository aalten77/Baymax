#ifndef LIGHT_H
#define LIGHT_H
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
class Light
{
public:
	GsVec pos;
	GsColor amb, dif, spe; //ambient, diffuse, specular
	Light() {}
	Light(const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s) : pos(p), amb(a), dif(d), spe(s) {}
	void set(const GsVec& p, const GsColor& a, const GsColor&d, const GsColor &s) { pos = p; amb = a; dif = d; spe = s; }
};
#endif // LIGHT_H