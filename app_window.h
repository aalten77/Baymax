
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_capsule.h"
# include "so_container.h"
# include "so_background.h"


// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
    GlShader _vertexsh, _fragsh, _vsh, _fsh;
    GlProgram _prog, _progtex;
    // My scene objects:
    SoAxis _axis;
	SoBackground _bkgrd;
	SoContainer _baymax; // added this - Ai-Linh
	//Rotations
	float rad, rad0;
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
	bool  _viewaxis, _viewbaymax, _viewbkgrd;
	int _cameraview;
    int _w, _h;
	int numfaces = 10; //num faces on capsule
	float rt = 0.5f; //top dome radius
	float rb = 0.5f; //bottom dome radius
	float len = 0.5f; // length of cylinder

	float mvBaymax_z = 0; //for up/down keys
	float mvBayMax_x = 0; //for left/right keys
	char mvBaymax_char;
	
	GsVec * eye = new GsVec[2];
	float angle = 0;

	bool otherway = false , otherway2 = false;


   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
	   virtual void glutIdle();
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void walk();
 };

#endif // APP_WINDOW_H
