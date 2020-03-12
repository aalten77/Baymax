# include <math.h>
# include "so_eyes.h"

#define PI 3.14159265

SoEyes::SoEyes()
{
	_numpoints = new int[2];
}

void SoEyes::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(4);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
}

void SoEyes::build()
{
	float nfaces = 15.0f;
	/************EYES******************/
	int i;
	P.clear(); C.clear(); // set size to zero, just in case
	P.reserve(nfaces * 243 / 2); C.reserve(nfaces * 243 / 2); // reserve space to avoid re-allocations from the calls below

	for (int i = 0; i <= nfaces; i++) {
		float theta = i * 2 * PI / nfaces;
		GsVec vec1 = GsVec(0.025f * cos(theta) , 0.025f * sin(theta), 0);
		P.push_back(vec1);
		C.push_back(GsColor::black);
	}

	_numpoints[0] = P.size();

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	// free non-needed memory:
	P.resize(0); C.resize(0);

	//**** LINE ****/

	P.push_back(GsVec(-0.055f, 0, 0)); C.push_back(GsColor::black);
	P.push_back(GsVec(0.055f, 0, 0)); C.push_back(GsColor::black);
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints[1] = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0);
}

void SoEyes::draw(GsMat& tr, GsMat& pr)
{
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	//THE EYES
	GsMat translEyeRight = GsMat();
	translEyeRight.translation(-0.055f, 0, 0);
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*translEyeRight).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_FAN, 0, _numpoints[0]);

	GsMat translEyeLeft = GsMat();
	translEyeLeft.translation(0.055f, 0, 0);
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*translEyeLeft).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glDrawArrays(GL_TRIANGLE_FAN, 0, _numpoints[0]);

	//THE LINE
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glDrawArrays(GL_LINES, 0, _numpoints[1]);

}
