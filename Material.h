#ifndef MATERIAL_H
#define MATERIAL_H
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
class Material
{
public:
	GsColor amb, dif, spe;
	float sh; //shininess
	Material(){}
	Material(const GsColor& a, const GsColor& d, const GsColor& s, float sn) : amb(a), dif(d), spe(s), sh(sn) {}
	void set(const GsColor& a, const GsColor&d, const GsColor &s, float sn) { amb = a; dif = d; spe = s; sh = sn; }
};

#endif // MATERIAL_H