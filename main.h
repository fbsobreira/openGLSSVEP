#ifndef MAIN_H
#define MAIN_H
//  Include GLUT, OpenGL, and GLU libraries
#include <GL/gl.h>

#ifdef __linux__ 
#include <GL/glx.h>
#include "glxext.h"	
PFNGLXSWAPINTERVALEXTPROC       glXSwapInterval = NULL;
#else
//#include <GL/glxew.h>
#include "wglext.h"	

extern PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT;
bool WGLExtensionSupported(const char *extension_name);
#endif




bool InitVSync();

//  Initialization
void init ();
void changeLayout(int initLayout);

//  Callback functions
void display (void);
void reshape (int w, int h);
void idle (void);


//  variable representing the window title
char *window_title = (char*)"Project: BME897 - SSVEP";

//  Tells whether to display the window full screen or not
//  Press Alt + Esc to exit a full screen.
int full_screen = 0;


//  The number of frames
int frameCount = 0;
//  Number of frames per second
float fps = 0;

//  currentTime - previousTime is the time elapsed
//  between every call of the Idle function
int currentTime = 0, previousTime = 0;

// Methods
void animateObject();
void drawObject(int selected, bool blink);
void calculateFPS();
void drawFPS();
void drawCLOCK();
void exec(char code);


#endif // MAIN_H

