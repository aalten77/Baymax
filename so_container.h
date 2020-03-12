#ifndef SO_CONTAINER_H
#define SO_CONTAINER_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so_capsule.h"
# include "so_eyes.h"
# include "Light.h"

#define M_PI 3.14159

class SoContainer : public GlObjects
{
private:

	GlShader _vsh, _fsh;
	GlProgram _proggouraud;

	SoCapsule _body;
	SoCapsule _leftleg;
	SoCapsule _rightleg;
	SoCapsule _head; 
	SoCapsule _leftarm;
	SoCapsule _rightarm;
	SoCapsule _elbowLeft; 
	SoCapsule _elbowRight;
	SoCapsule _fingers;
	SoEyes _eyes;

	Light _light;

	
	GsMat mvShoulder = GsMat();
	float radtastic = M_PI / 2; //degree for head
	float radtastic2 = M_PI / 6;
	float radtastic3 = 0; //degree for body
	float radtastic4 = M_PI/6; //degree for left shoulder
	float radtastic5 = -M_PI/6;  //degree for right shoulder
	float radtastic6 = 0; 
	float radtastic7 = 0;  //degree for left elbow
	float radtastic8 = 0;  //degree for right elbow
	float radtastic9 = 0;  //degree for left leg
	float radtastic10 = 0;  //degree for right leg

	

	GsMat rotBay = GsMat();

public:

	SoContainer();
	void init(const GlProgram& prog);
	void build();
	void draw(GsMat& tr, GsMat& pr, float colorMult, GsMat& mvTrans, char mv);
	void setMvBody(float rad);
	float getRadMvBody();
	void setMvLeftArm(float rad);
	float getRadMvLeftArm();
	void setMvRightArm(float rad);
	float getRadMvRightArm();
	void setMvLeftElbow(float rad);
	float getRadMvLeftElbow();
	void setMvRightElbow (float rad);
	float getRadMvRightElbow();
	void setMvLeftLeg(float rad);
	float getRadMvLeftLeg();
	void setMvRightLeg(float rad);
	float getRadMvRightLeg();
};

#endif // SO_CONTAINER_H