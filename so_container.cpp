
#define _USE_MATH_DEFINES

#include <cmath>
#include "so_container.h"

SoContainer::SoContainer() {

}

void SoContainer::init(const GlProgram& prog) {

	//Gouraud shader (replace the flat shader)
	_vsh.load_and_compile(GL_VERTEX_SHADER, "../vsh_flat.glsl");
	_fsh.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_flat.glsl");
	_proggouraud.init_and_link(_vsh, _fsh);

	_body.init(_proggouraud);
	_leftleg.init(_proggouraud);
	_rightleg.init(_proggouraud);
	_head.init(_proggouraud);
	_leftarm.init(_proggouraud);
	_rightarm.init(_proggouraud);
	_elbowLeft.init(_proggouraud);
	_elbowRight.init(_proggouraud);
	_eyes.init(prog);

	_light.set(GsVec(10.0f, 30.0f, 30.0f), GsColor(50, 50, 50, 255), GsColor::gray, GsColor::white);

}

void SoContainer::build() {
	_body.build(0.5f, 0.25f, 0.35f);
	_leftleg.build(0.25f, 0.15f, 0.15f);
	_rightleg.build(0.25f, 0.15f, 0.15f);
	_head.build(0.05f, 0.1f, 0.1f);
	_leftarm.build(0.45f/2.0f, 0.1f, 0.11f);
	_rightarm.build(0.45f/2.0f, 0.1f, 0.11f);
	_elbowLeft.build(0.45f / 2.0f, 0.11f, 0.12f);
	_elbowRight.build(0.45f / 2.0f, 0.11f, 0.12f);
	_eyes.build();

}

void SoContainer::draw(GsMat& tr, GsMat& pr, float colorMult, GsMat& mvTrans, char mv){
	//set rotation for Baymax when walking
	GsMat rt = GsMat();
	if (mv == 'u') {
		rt.roty(M_PI);
		rotBay = rotBay.inverse();
		rotBay *= rt;
	}
	else if (mv == 'd') {
		rt.roty(0);
		rotBay = rotBay.inverse();
		rotBay *= rt;
	}
	else if (mv == 'l') {
		rt.roty((3*M_PI)/2);
		rotBay = rotBay.inverse();
		rotBay *= rt;
	}
	else if (mv == 'r') {
		rt.roty(M_PI/2);
		rotBay = rotBay.inverse();
		rotBay *= rt;
	}
	else {
		rt = GsMat(); 
		rotBay *= rt; 
	}


	GsMat modelMatrix = GsMat()*mvTrans*rotBay;
	//BODY
	GsMat translOrigin = GsMat(); translOrigin.translation(0.0f, -0.3f, 0.0f);
	GsMat rotateBody = GsMat();
	GsMat translBody = GsMat(); translBody.translation(0.0f, 0.3f, 0.0f);
	rotateBody.rotz(radtastic3);
	modelMatrix *= translOrigin* rotateBody * translBody;
	_body.draw(tr*modelMatrix, pr, _light, colorMult);

	//HEAD
	GsMat headMat = modelMatrix;
	GsMat translUp = GsMat();
	GsMat rotate = GsMat(); rotate.rotz(radtastic);
	translUp.translation(0, 0.55f, 0);
	headMat *= (translUp*rotate);
	_head.draw(tr*headMat, pr, _light, colorMult);

	//LEFTARM
	GsMat leftArmMat = modelMatrix;
	GsMat translUpLArm = GsMat(); translUpLArm.translation(0.2f, 0.35f, 0);
	GsMat translDown3 = GsMat(); translDown3.translation(0.0f, -0.15f, 0);
	GsMat rotate2 = GsMat(); rotate2.rotz(radtastic4);
	leftArmMat *= translUpLArm * rotate2 * translDown3;
	_leftarm.draw(tr*leftArmMat , pr, _light, colorMult);

	//LEFT ELBOW
	GsMat leftElbow = leftArmMat;
	GsMat translDown5 = GsMat(); translDown5.translation(0, -0.15f, 0.0f);
	GsMat rotateLElbow = GsMat(); rotateLElbow.rotx(-radtastic7);
	GsMat translLElbow = GsMat(); translLElbow.translation(0.0f, -0.12f, 0.0f);
	leftElbow *= (translDown5 * rotateLElbow * translLElbow);
	_elbowLeft.draw(tr*leftElbow, pr, _light, colorMult);

	
	//RIGHTARM
	GsMat rightArmMat = modelMatrix;
	GsMat translUpRArm = GsMat(); translUpRArm.translation(-0.2f, 0.35f, 0);
	GsMat translDown4 = GsMat(); translDown4.translation(0.0f, -0.15f, 0);
	GsMat rotate3 = GsMat(); rotate3.rotz(radtastic5);
	rightArmMat *= translUpRArm * rotate3 * translDown4;
	_rightarm.draw(tr*rightArmMat, pr, _light, colorMult);

	//RIGHT ELBOW
	GsMat rightElbow = rightArmMat;
	GsMat translDown6 = GsMat(); translDown6.translation(0, -0.15f, 0.0f);
	GsMat rotateRElbow = GsMat(); rotateRElbow.rotx(radtastic8);
	GsMat translRElbow = GsMat(); translRElbow.translation(0.0f, -0.12f, 0.0f);
	rightElbow *= (translDown6 * rotateRElbow * translRElbow);
	_elbowRight.draw(tr*rightElbow, pr, _light, colorMult);


	//EYES
	GsMat eyesMat = modelMatrix;
	GsMat translOut = GsMat();
	translOut.translation(0, -0.01f, 0.10001f);
	eyesMat *= (translUp*translOut);
	_eyes.draw(tr*eyesMat, pr);

	//LEFTLEG
	GsMat leftLeg = GsMat();
	GsMat translDown = GsMat();
	translDown.translation(0.15f, -0.60f, 0);
	GsMat rotLleg = GsMat(); rotLleg.rotx(radtastic9);
	leftLeg *= (rotLleg*translDown );
	_leftleg.draw(tr*mvTrans*rotBay*leftLeg, pr, _light, colorMult);

	//RIGHTLEG
	GsMat rightLeg = GsMat();
	GsMat translDown2 = GsMat();
	translDown2.translation(-0.15f, -0.60f, 0);
	GsMat rotRleg = GsMat(); rotRleg.rotx(radtastic10);
	rightLeg *= (rotRleg*translDown2);
	_rightleg.draw(tr*mvTrans*rotBay*rightLeg, pr, _light, colorMult);

}

void SoContainer::setMvBody(float rad) {
	float oldradtastic = radtastic3;
	radtastic3 = rad;

	if (radtastic3 < -M_PI / 12 || radtastic3 > M_PI / 12) {
		radtastic3 = oldradtastic;
	}
		
}

float SoContainer::getRadMvBody() {
	return radtastic3;
}

void SoContainer::setMvLeftArm(float rad) {
	radtastic4 = rad;

}

float SoContainer::getRadMvLeftArm() {
	return radtastic4;
}

void SoContainer::setMvRightArm(float rad) {
	radtastic5 = rad;

}

float SoContainer::getRadMvRightArm() {
	return radtastic5;
}

void SoContainer::setMvLeftElbow(float rad) {
	radtastic7 = rad;

}

float SoContainer::getRadMvLeftElbow() {
	return radtastic7;
}

void SoContainer::setMvRightElbow(float rad) {
	radtastic8 = rad;

}

float SoContainer::getRadMvRightElbow() {
	return radtastic8;
}

void SoContainer::setMvLeftLeg(float rad) {
	radtastic9 = rad;

}

float SoContainer::getRadMvLeftLeg() {
	return radtastic9;
}

void SoContainer::setMvRightLeg(float rad) {
	radtastic10 = rad;
}

float SoContainer::getRadMvRightLeg() {
	return radtastic10;
}


