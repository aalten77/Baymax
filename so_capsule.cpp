# include <math.h>
# include "so_capsule.h"

#define PI 3.14159265

SoCapsule::SoCapsule()
{
	_numpoints = new int[2];
}

void SoCapsule::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(6);       // will use 2 buffers: one for coordinates and one for colors and one for normals
	uniform_locations(9);
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	uniform_location(2, "lPos");
	uniform_location(3, "la");
	uniform_location(4, "ld");
	uniform_location(5, "ls");
	uniform_location(6, "ks");
	uniform_location(7, "sh");
	uniform_location(8, "colorScale");
}

void SoCapsule::build(float len, float rt, float rb)
{

	float nfaces = 25.0f;

	int i;
	P.clear(); C.clear(); N.clear();// set size to zero, just in case
	//P.reserve(nfaces*243/2); C.reserve(nfaces * 243 / 2); // reserve space to avoid re-allocations from the calls below
	//int Pvectors = P.size();
	GsMat translTop = GsMat();
	translTop.translation(0.0f, len / 2.0f, 0.0f);
	GsMat scaleTop = GsMat();
	scaleTop.scale(rt, rt, rt);

	transformationTop = translTop*scaleTop;

	GsMat translBot = GsMat();
	translBot.translation(0.0f, -(len / 2.0f), 0.0f);
	GsMat scaleBot = GsMat();
	scaleBot.scale(rb, -rb, rb);

	transformationBottom = translBot*scaleBot;
	
	////****** TOP DOME ******
	const int totalFaces = (nfaces + 3) / 4;

	for (int i = 0; i < totalFaces; i++) {
		float theta = ((float)i)*PI / 2.0 / totalFaces;
		float nextTheta = ((float)i + 1)*PI / 2.0 / totalFaces;
		GsVec vec1 = GsVec(cos(theta), sin(theta), 0);
		GsVec vec2 = GsVec(cos(nextTheta), sin(nextTheta), 0);

		for (int j = 0; j <= nfaces; j++) {
			float triTheta = j * 2.0*PI / nfaces;
			GsMat rotationMat = GsMat(); rotationMat.roty(triTheta);
			GsPnt a = rotationMat*vec1; GsPnt b = rotationMat*vec2;
			P.push_back(a); C.push_back(GsColor::white);
			P.push_back(b); C.push_back(GsColor::white);
			triTheta = (j + 1) * 2.0*PI / nfaces;
			rotationMat = GsMat(); rotationMat.roty(triTheta);
			GsPnt c = rotationMat*vec1; GsPnt d = rotationMat*vec2;
			P.push_back(c); C.push_back(GsColor::white);

			N.push_back(GsVec(a)); N.push_back(GsVec(b)); N.push_back(GsVec(c));

			P.push_back(b); C.push_back(GsColor::white);
			P.push_back(c); C.push_back(GsColor::white);
			P.push_back(d); C.push_back(GsColor::white);

			N.push_back(GsVec(b)); N.push_back(GsVec(c)); N.push_back(GsVec(d));
		}
	}

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*N.size(), &N[0], GL_STATIC_DRAW);


	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints[0] = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0); N.resize(0);

	//**** CYLINDER ****/
	for (int i = 0; i < nfaces; i++) {
		float theta = ((float)i)*2.0*PI / nfaces;
		float nextTheta = ((float)i + 1)*2.0*PI / nfaces;
		GsPnt a = GsPnt(rt*cos(theta), len / 2.0f, rt*sin(theta));
		GsPnt b = GsPnt(rb*cos(theta), -(len / 2.0f), rb*sin(theta));
		GsPnt c = GsPnt(rt*cos(nextTheta), len / 2.0f, rt*sin(nextTheta));
		GsPnt d = GsPnt(rb*cos(nextTheta), -(len / 2.0f), rb*sin(nextTheta));
		P.push_back(a); P.push_back(b); P.push_back(c); C.push_back(GsColor::white); C.push_back(GsColor::white); C.push_back(GsColor::white);
		P.push_back(c); P.push_back(b); P.push_back(d); C.push_back(GsColor::white); C.push_back(GsColor::white); C.push_back(GsColor::white);

		GsVec vec1 = GsVec(cos(theta), 0, sin(theta));
		GsVec vec2 = GsVec(cos(nextTheta), 0, sin(nextTheta));

		N.push_back(vec1); N.push_back(vec1); N.push_back(vec2);
		N.push_back(vec2); N.push_back(vec1); N.push_back(vec2);
	}
	
	
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[4]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[5]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*N.size(), &N[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints[1] = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0); N.resize(0);
}

void SoCapsule::draw(GsMat& tr, GsMat& pr, const Light& l, float colorMult)
{
	const float m_shi = 0.5f;
	float f[4];
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	//DOME TOP
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*transformationTop).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);
	glUniform3fv(uniloc[2], 1, l.pos.e);
	glUniform4fv(uniloc[3], 1, l.amb.get(f));
	glUniform4fv(uniloc[4], 1, l.dif.get(f));
	glUniform4fv(uniloc[5], 1, l.spe.get(f));
	glUniform4fv(uniloc[6], 1, GsColor::gray.get(f));
	glUniform1fv(uniloc[7], 1, &m_shi);
	glUniform1f(uniloc[8],colorMult);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints[0]);

	//DOME BOTTOM
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr*transformationBottom).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);
	//glUniform3fv(uniloc[2], 1, l.pos.e);
	//glUniform4fv(uniloc[3], 1, l.amb.get(f));
	//glUniform4fv(uniloc[4], 1, l.dif.get(f));
	//glUniform4fv(uniloc[5], 1, l.spe.get(f));
	//glUniform4fv(uniloc[6], 1, GsColor::gray.get(f));
	//glUniform1fv(uniloc[7], 1, &m_shi);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints[0]);

	//THE CYLINDER
	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[4]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[5]); // normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, (tr).e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);
	//glUniform3fv(uniloc[2], 1, l.pos.e);
	//glUniform4fv(uniloc[3], 1, l.amb.get(f));
	//glUniform4fv(uniloc[4], 1, l.dif.get(f));
	//glUniform4fv(uniloc[5], 1, l.spe.get(f));
	//glUniform4fv(uniloc[6], 1, GsColor::gray.get(f));
	//glUniform1fv(uniloc[7], 1, &m_shi);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints[1]);

}
