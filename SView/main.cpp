//*************************************************************************
//
//  Project     : BME897 - SSVEP
//  Description	:
//
//*************************************************************************

//  Standard Input\Output C Library
#include <stdio.h>
//  To use functions with variables arguments
#include <stdarg.h>
#include <stdarg.h>
#include <math.h>
//  Include GLUT, OpenGL, and GLU libraries
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <chrono>
#include "../udp_server.h" // For UDPSocket and SocketException
#include <iostream>          // For cout and cerr
#include <cstdlib>           // For atoi()

#include "../FreeType.h"
freetype::font_data our_font;
freetype::font_data our_fontBig;


#ifdef __linux__ 
#include <GL/glx.h>
#include "../glxext.h"	
PFNGLXSWAPINTERVALEXTPROC       glXSwapInterval = NULL;
#else
//#include <GL/glxew.h>
#include "../wglext.h"	
extern PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT;
bool WGLExtensionSupported(const char *extension_name);
#endif

bool InitVSync();

//  Initialization
void init ();
//  Callback functions
void display (void);
void reshape (int w, int h);
void idle (void);

// Windows ID
GLint WindowID1;
//  variable representing the window title
char *window_title = (char*)"Project: BME897 - SSVEP";

//  Tells whether to display the window full screen or not
//  Press Alt + Esc to exit a full screen.
int full_screen = 0;


int SCREEN_SIZE_X=1600;
int SCREEN_SIZE_Y=900;

//UDP Buffer
char *UDP_BUFFER = new char[100];
// Start UDP Server
udp_server *UDP;

//  variables representing the window size
int screenWindow_x;
int screenWindow_y;
int window_width;
int window_height;
void centerOnScreen ()
{
    screenWindow_x = (glutGet (GLUT_SCREEN_WIDTH) - window_width)/2;
    screenWindow_y = (glutGet (GLUT_SCREEN_HEIGHT) - window_height)/2;
}

//-------------------------------------------------------------------------
//  Set OpenGL program initial state.
//-------------------------------------------------------------------------
void init ()
{
    //  Set the frame buffer clear color to black.
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);		// Enable Texture Mapping
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);			// The Type Of Depth Test To Do
    glShadeModel(GL_SMOOTH);		// Enables Smooth Color Shading
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix
    glMatrixMode(GL_MODELVIEW);
	
	our_font.init("../Test.ttf", 16);
	our_fontBig.init("../Test.ttf", 30);
	UDP_BUFFER[0]='\0';
}

//-------------------------------------------------------------------------
//  This function is passed to glutDisplayFunc in order to display
//	OpenGL contents on the window.
//-------------------------------------------------------------------------
void display (void)
{
	// Clear buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	//Set Color to Red
	glColor3f(1.0f,0.0f,0.0f); 
	glScalef(1.5,1.5,1);
	freetype::print(our_fontBig, 0, 0, "%s", UDP_BUFFER);
	
	
	glPopMatrix();
	//Swap buffer to screen
    glutSwapBuffers ();
	glFlush();
	
}

void idle (void)
{	
	//Verify UDP Buffer
	int rec = UDP->timed_recv(UDP_BUFFER,100,100);
	if (rec>0) {
		//UDP_BUFFER[rec]=0;
		printf("T:%d: %.*s\n",rec,rec, UDP_BUFFER);
		UDP_BUFFER[rec]='\0';
	}
	/*if (UDP->HAS_DATA()){
		
	}*/

    //  Call display function (draw the current frame)
    glutPostRedisplay ();

    glFlush();
    glFinish();
}

//-------------------------------------------------------------------------
//  This function is passed to the glutReshapeFunc and is called
//  whenever the window is resized.
//-------------------------------------------------------------------------
void reshape (int w, int h)
{
	window_width = w;
    window_height = h;
	
	int scaleX = floor((w-SCREEN_SIZE_X)/2);
	int scaleY = floor((h-SCREEN_SIZE_Y)/2);

    // Scale to the Screen Size
    glViewport(floor(w-SCREEN_SIZE_X-scaleX)/2, floor(h-SCREEN_SIZE_Y-scaleY)/2, SCREEN_SIZE_X+scaleX, SCREEN_SIZE_Y+scaleY);
}

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
//-------------------------------------------------------------------------
//  This function is passed to the glutKeyboardFunc and is called
//  whenever a key is pressed.
//-------------------------------------------------------------------------
static void Key(unsigned char key, int x, int y)
{

    switch (key) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		break;
		case '0':
			if (full_screen){
				glutReshapeWindow(SCREEN_SIZE_X, SCREEN_SIZE_Y);
				centerOnScreen ();
				full_screen=false;
			}else{
				glutFullScreen();
				full_screen=true;
			}
			
		break;
		case 27:
			//glutLeaveMainLoop();
			glutExit();
			//exit(0);
    }
	// RE-Display
    glutPostRedisplay();
}


//-------------------------------------------------------------------------
//  Program Main method.
//-------------------------------------------------------------------------
int main (int argc, char **argv)
{

    //  variables representing the window size
    int window_width = SCREEN_SIZE_X;
    int window_height = SCREEN_SIZE_Y;

	// Define fonte style
    font_style = GLUT_BITMAP_9_BY_15;

    //  Connect to the windowing system
    glutInit(&argc, argv);

    //  create a window with the specified dimensions
    glutInitWindowSize (window_width, window_height);
    
	centerOnScreen ();
    //  Position Window
    glutInitWindowPosition (screenWindow_x, screenWindow_y);
    //  Set Display mode
    glutInitDisplayMode (GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_ACCUM);
    //  Create window with the specified title
    WindowID1=glutCreateWindow (window_title);

    //  View in full screen if the full_screen flag is on
    if (full_screen)
        glutFullScreen ();

    InitVSync();

    #ifndef __linux__
    wglSwapIntervalEXT(1);
    #else
    glXSwapInterval(glXGetCurrentDisplay(),glXGetCurrentReadDrawable(),1);
    #endif

    //  Set OpenGL program initial state.
    init();

    // Set the callback functions
    glutDisplayFunc (display);
    glutReshapeFunc  (reshape);
	glutKeyboardFunc(Key);
    glutIdleFunc (idle);
	
	//Init UDP
	UDP = new udp_server("0.0.0.0", 1051);
	
	//  Start GLUT event processing loop
	glutMainLoop();
	
}

#ifdef __linux__ 
bool InitVSync(){
	// Extension is supported, init pointers.
    glXSwapInterval = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");
    return true;
}
#else
bool InitVSync(){
    if (WGLExtensionSupported("WGL_EXT_swap_control"))
    {
        // Extension is supported, init pointers.
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");

        // this is another function from WGL_EXT_swap_control extension
        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
        return true;
    }
    return false;
}
#endif
