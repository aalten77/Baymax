#define _USE_MATH_DEFINES

#include <cmath>
# include "so_background.h"

SoBackground::SoBackground()
{
	_numpoints = 0;
}

void SoBackground::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(2); // will use 1 vertex array
	gen_buffers(4);       // will use 2 buffers: one for coordinates and one for texture
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");

	GsImage I;

	if (!I.load("../tadashilab.jpg")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }

	glBindVertexArray(va[0]);
	glGenTextures(1, &_texid);
	glBindTexture(GL_TEXTURE_2D, _texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	I.init(0, 0); // free image from CPU

	GsImage I2;
	if (!I2.load("../baymaxwall.jpg")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }

	glBindVertexArray(va[0]);
	glGenTextures(1, &_texid2);
	glBindTexture(GL_TEXTURE_2D, _texid2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, I2.w(), I2.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I2.data());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	I2.init(0, 0); // free image from CPU
}

void SoBackground::build()
{
	int i;

	P.clear(); T.clear(); // set size to zero, just in case
	//P.reserve(6); C.reserve(6); // reserve space to avoid re-allocations from the calls below

	GsVec a = GsVec(-10.0f, 10.0f, -10.0f);
	GsVec b = GsVec(10.0f, 10.0f, -10.0f);
	GsVec c = GsVec(10.0f, -0.85f, -10.0f);
	GsVec d = GsVec(-10.0f, -0.85f, -10.0f);

	GsVec2 tA = GsVec2(0, 0);
	GsVec2 tB = GsVec2(1.0f, 0);
	GsVec2 tC = GsVec2(1.0f, 1.0f);
	GsVec2 tD = GsVec2(0, 1.0f);

	P.push_back(a); P.push_back(b); P.push_back(d);
	P.push_back(b); P.push_back(c); P.push_back(d);
	T.push_back(tA); T.push_back(tB); T.push_back(tD);
	T.push_back(tB); T.push_back(tC); T.push_back(tD);
	//for (i = 0; i < 6; i++) C.push_back(GsColor::white);

	glBindVertexArray(va[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);


	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); T.resize(0);

	a = GsVec(-10.0f, -0.85f, -10.0f);
	b = GsVec(10.0f, -0.85f, -10.0f);
	c = GsVec(10.0f, -0.85f, 10.0f);
	d = GsVec(-10.0f, -0.85f, 10.0f);

	tA = GsVec2(0, 0);
	tB = GsVec2(1.0f, 0);
	tC = GsVec2(1.0f, 1.0f);
	tD = GsVec2(0, 1.0f);

	P.push_back(a); P.push_back(b); P.push_back(d);
	P.push_back(b); P.push_back(c); P.push_back(d);
	T.push_back(tA); T.push_back(tB); T.push_back(tD);
	T.push_back(tB); T.push_back(tC); T.push_back(tD);
	//for (i = 0; i < 6; i++) C.push_back(GsColor::white);


	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);


	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); T.resize(0);
}

void SoBackground::draw(GsMat& tr, GsMat& pr)
{
	// Draw Lines:
	glUseProgram(prog);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_2D, _texid);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	//wall 1

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GsMat rotation = GsMat();  rotation.roty(M_PI / 2);
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*rotation).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	// wall 2
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	rotation.roty(M_PI);
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*rotation).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	// wall 3
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	rotation.roty(-M_PI/2);
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*rotation).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	//FLOOR
	glBindTexture(GL_TEXTURE_2D, _texid2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}

