
# include <iostream>
#include <cmath>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = _viewbaymax = _viewbkgrd = true;
   _cameraview = 0;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;
 }

void AppWindow::initPrograms ()
 {
   // Load your shaders and link your programs here:
	//flat shader
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "../3dcolored.vert" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "../flat.frag" );
   _prog.init_and_link ( _vertexsh, _fragsh );

   //flat shader with texture
   _vsh.load_and_compile(GL_VERTEX_SHADER, "../texflat.vert");
   _fsh.load_and_compile(GL_FRAGMENT_SHADER, "../texflat.frag");
   _progtex.init_and_link(_vsh, _fsh);



   // Init my scene objects:
   _axis.init ( _prog );
   _bkgrd.init(_progtex);
   _baymax.init(_prog);
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
	const float incf = 0.05f;
	const float incr = GS_TORAD(2.5f);

   switch ( key )
    { 
	   //toggle axis
		case '/': _viewaxis = !_viewaxis; redraw(); break;

	   //toggle birdseye view
		case ' ': 
			if (_cameraview == 0) {
				_cameraview = 1;
			}
			else {
				_cameraview = 0; 
			}
			redraw();
			break;

		//arrow keys
		case '4': _roty -= incr; break; //L
		case '6': _roty += incr; break; //R
		case '8': _rotx -= incr; break; //U
		case '2': _rotx += incr; break; //D

		//1st DOF: body
		case 'q': rad = _baymax.getRadMvBody();
			rad -= (M_PI / 36);
			_baymax.setMvBody(rad);
			redraw();
			break;
		case 'a': rad = _baymax.getRadMvBody();
			rad += (M_PI / 36);
			_baymax.setMvBody(rad);
			redraw(); break;
		//2nd DOF: Right shoulder
		case 'w': rad = _baymax.getRadMvRightArm();
			rad -= (M_PI / 36);
			_baymax.setMvRightArm(rad);
			redraw(); break;
		case 's': rad = _baymax.getRadMvRightArm();
			rad += (M_PI / 36);
			_baymax.setMvRightArm(rad);
			redraw(); break;
		//3rd DOF: Left shoulder
		case 'e': rad = _baymax.getRadMvLeftArm();
			rad += (M_PI / 36);
			_baymax.setMvLeftArm(rad);
			redraw(); break;
		case 'd': rad = _baymax.getRadMvLeftArm();
			rad -= (M_PI / 36);
			_baymax.setMvLeftArm(rad);
			redraw(); break;
		//4th DOF: Right elbow
		case 'r': rad = _baymax.getRadMvRightElbow();
			rad -= (M_PI / 36);
			_baymax.setMvRightElbow(rad);
			redraw(); break;
		case 'f': rad = _baymax.getRadMvRightElbow();
			rad += (M_PI / 36);
			_baymax.setMvRightElbow(rad);
			redraw(); break;
		//5th DOF: Left elbow
		case 't': rad = _baymax.getRadMvLeftElbow ();
			rad += (M_PI / 36);
			_baymax.setMvLeftElbow(rad);
			redraw(); break;
		case 'g': rad = _baymax.getRadMvLeftElbow();
			rad -= (M_PI / 36);
			_baymax.setMvLeftElbow(rad);
			redraw(); break;
		case 27 : exit(1); // Esc was pressed
	}
 }

void AppWindow::glutIdle()
{
	//change eye - ground level/birds eye
	static long lastDraw = 0;
	long currentTime = glutGet(GLUT_ELAPSED_TIME);
	if ((currentTime - lastDraw) > (1000 / 60)) {
		angle += M_PI / 36;
		eye[0] = GsVec(0, 0, 2);
		eye[1] = GsVec(4 * cos(angle), 15, 4 * sin(angle));

		redraw();
	}

}

void AppWindow::walk() {
	//Move left and right leg 
	rad = _baymax.getRadMvLeftLeg();
	rad0 = _baymax.getRadMvRightLeg();
	if (rad < -M_PI / 12) {
		rad += (M_PI / 36);
		otherway = true;
	}
	else if (rad > M_PI / 12) {
		rad -= (M_PI / 36);
		otherway = false;
	}
	else {
		if (otherway)
			rad += (M_PI / 36);
		else
			rad -= (M_PI / 36);
	}
	if (rad0 < -M_PI / 12) {
		rad0 += (M_PI / 36);
		otherway2 = false;
	}
	else if (rad0 > M_PI / 12) {
		rad0 -= (M_PI / 36);
		otherway2 = true;
	}
	else {
		if (otherway2)
			rad0 -= (M_PI / 36);
		else
			rad0 += (M_PI / 36);
	}

	_baymax.setMvLeftLeg(rad);
	_baymax.setMvRightLeg(rad0);
}

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
   {
	   //walk character
	  case GLUT_KEY_LEFT:      mvBayMax_x -= 0.025f; mvBaymax_char = 'l'; walk(); break;//_roty-=incr; break;
	  case GLUT_KEY_RIGHT:     mvBayMax_x += 0.025f; mvBaymax_char = 'r'; walk(); break;//_roty+=incr; break;
	  case GLUT_KEY_UP:        mvBaymax_z -= 0.025f; mvBaymax_char = 'u'; walk(); break; //_rotx-=incr; break;
	  case GLUT_KEY_DOWN:      mvBaymax_z += 0.025f; mvBaymax_char = 'd'; walk(); break; //_rotx+=incr; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      case GLUT_KEY_HOME:      _fovy=GS_TORAD(60.0f); _rotx=_roty=0; break;
      default: return; // return without rendering
	}
   if(rd) redraw(); // ask the window to be rendered when possible

 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
   { 
	   _axis.build(1.0f); // axis has radius 1.0
   }

   if (_bkgrd.changed) _bkgrd.build();

   if (_baymax.changed) {
	   _baymax.build(); //added this - Ai-Linh 
   }

   // Define our scene transformation:
   GsMat rx, ry, stransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   //GsVec eye(0, 0, 2);
   //****change camera view here****
   GsVec center(0, 0, 0), up(0, 1, 0);
	camview.lookat(eye[_cameraview], center, up);
    // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause the values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if (_viewaxis) { 
	   _axis.draw(stransf, sproj); 
   }

   if (_viewbkgrd) _bkgrd.draw(stransf, sproj);

   GsMat transMat = GsMat(); transMat.translation(mvBayMax_x, 0, mvBaymax_z);
   if (_viewbaymax) {
	   _baymax.draw(stransf, sproj, 1.0f, transMat, mvBaymax_char); // added this - Ai-Linh 
   }

   //*********DRAW SHADOW**************
   float * light = new float[4];
   light[0] = -0.5f; light[1] = -0.5f, light[2] = -0.5f, light[3] = 0.0f;
   float * ground = new float[4];
   ground[0] = 0.0f; ground[1] = -1.0f, ground[2] = 0.0f, ground[3] = -0.84f;
   /* GsMat mvdwn = GsMat();
   mvdwn.translation(0.0f, -0.5f, 0.0f);
   ground = mvdwn*ground;*/

   float dot = ground[0] * light[0] + ground[1] * light[1] + ground[2] * light[2] + ground[3] * light[3];
   GsMat shadowMat = GsMat(
	   dot - light[0] * ground[0],
	   0.0f - light[0] * ground[1],
	   0.0f - light[0] * ground[2],
	   0.0f - light[0] * ground[3],

	   0.0f - light[1] * ground[0],
	   dot - light[1] * ground[1],
	   0.0f - light[1] * ground[2],
	   0.0f - light[1] * ground[3],

	   0.0f - light[2] * ground[0],
	   0.0f - light[2] * ground[1],
	   dot - light[2] * ground[2],
	   0.0f - light[2] * ground[3],

	   0.0f - light[3] * ground[0],
	   0.0f - light[3] * ground[1],
	   0.0f - light[3] * ground[2],
	   dot - light[3] * ground[3]
   );

   GsMat mvdwn = GsMat();
   mvdwn.translation(0.0f, -0.84f, 0.0f);
   _baymax.draw(stransf*shadowMat, sproj, 0.0f, transMat, mvBaymax_char);
   

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

