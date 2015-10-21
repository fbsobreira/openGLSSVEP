#ifndef MAIN_H
#define MAIN_H
//  Include GLUT, OpenGL, and GLU libraries
#include <GL/gl.h>
#include <GL/glx.h>
#ifdef __linux__ 
#include "glxext.h"	

PFNGLXSWAPINTERVALEXTPROC       glXSwapInterval = NULL;
/*Display *	glXGetCurrentDisplay = NULL;
GLXDrawable glXGetCurrentReadDrawable = NULL;
*/
// VSync for Windows

bool InitVSync(){
	// Extension is supported, init pointers.
        glXSwapInterval = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");
/*        glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAYEXTPROC) glXGetProcAddress((const GLubyte *)"glXGetCurrentDisplayEXT");
        glXGetCurrentReadDrawable = (PFNGLXGETCURRENTDISPLAYEXTPROC) glXGetProcAddress((const GLubyte *)"glXGetCurrentReadDrawable");
*/

        
        return true;
}

#else
#include "wglext.h"	

PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

// VSync for Windows
bool WGLExtensionSupported(const char *extension_name)
{
    // this is pointer to function which returns pointer to string with list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

    // determine pointer to wglGetExtensionsStringEXT function
    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");


    if (_wglGetExtensionsStringEXT==NULL) return FALSE;
    // extension is supported
    return true;
}

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

//  Initialization
void init ();

//  Callback functions
void display (void);
void reshape (int w, int h);
void idle (void);


//  variable representing the window title
char *window_title = (char*)"Project: BME897 - SSVEP";

//  Tells whether to display the window full screen or not
//  Press Alt + Esc to exit a full screen.
int full_screen = 1;


//  The number of frames
int frameCount = 0;
//  Number of frames per second
float fps = 0;

//  currentTime - previousTime is the time elapsed
//  between every call of the Idle function
int currentTime = 0, previousTime = 0;

// Methods
void animateObject();
void drawObject();
void calculateFPS();
void drawFPS();


#endif // MAIN_H

