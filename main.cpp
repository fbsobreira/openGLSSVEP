
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
#include <vector>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>

#include "ssvepobject.h"
#include "glhelper.h"
#include "layoutView.h"

#include "main.h"

#include <chrono>
#include "udp_server.h" // For UDPSocket and SocketException
#include <iostream>          // For cout and cerr
#include <cstdlib>           // For atoi()


#include "FreeType.h"
freetype::font_data our_font;

int DRAWWITHPICTURES=1;
int DRAWFREQ=0;
int DRAWFPS=0;
int DRAWCLOCK=1;
int SCREEN_SIZE_X=1920;
int SCREEN_SIZE_Y=1080;

char *UDP_BUFFER = new char[100];
// Start UDP Server
udp_server UDP = udp_server("0.0.0.0", 5000);

int feedback_box=0;
int feedback_box_count=0;


//Templete for Size
template<class T, size_t N>
size_t size(T (&)[N]) { return N; }

//List of SSVEP objects
std::vector<SSVEPObject> oList;

std::vector<layoutView> layoutList;

//Generate Positon based on Polar Coordinates
void getPos(GLfloat * V2Pos, GLfloat R, GLfloat angle, GLfloat Size){
    angle = angle * 3.14 / 180.0 ;
    V2Pos[0] = R*(GLfloat)sin(angle)-Size/2;
    V2Pos[1] = R*(GLfloat)cos(angle)+Size/2;
    V2Pos[2] = R*(GLfloat)sin(angle)+Size/2;
    V2Pos[3] = R*(GLfloat)cos(angle)-Size/2;
    return;
}

	unsigned int NLayouts = 2;
	unsigned int NBlocks = 8;
		

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
	
	//Create Layout 1
	layoutList.push_back(layoutView(8));
	layoutList.back().setAngles((const GLfloat[]){0,38,90,142,180,218,270,322});
	layoutList.back().setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back().setR((const GLfloat[]){0.7,0.75,0.8,0.75,0.7,0.75,0.8,0.75});
	layoutList.back().setFrequencies((const GLfloat[]){10,4.6,5.4,6.6,8.5,5,6,7.5});
	printf("Init Test\n");
	layoutList.back().setImages((const char[][25]){{"NurseButton.png"},{"SpellerButton.png"},
		{"NoButton.png"},{"CommonNeedsButton.png"},{"SleepModeButton.png"},{"MediaControlButton.png"},
		{"YesButton.png"},{"MedicalNeedsButton.png"}});
	layoutList.back().setImagesN((const char[][25]){{"NurseButtonN.png"},{"SpellerButtonN.png"},
		{"NoButtonN.png"},{"CommonNeedsButtonN.png"},{"SleepModeButtonN.png"},{"MediaControlButtonN.png"},
		{"YesButtonN.png"},{"MedicalNeedsButtonN.png"}});
	layoutList.back().initObjects();
	
	our_font.init("Test.ttf", 16);
    
}


void DrawCubeFace(float fSize)
{
  fSize /= 2.0;
  glBegin(GL_QUADS);
  glVertex3f(-fSize, -fSize, fSize);    glTexCoord2f (0, 0);
  glVertex3f(fSize, -fSize, fSize);     glTexCoord2f (1, 0);
  glVertex3f(fSize, fSize, fSize);      glTexCoord2f (1, 1);
  glVertex3f(-fSize, fSize, fSize);     glTexCoord2f (0, 1);
  glEnd();
}
void DrawCubeWithTextureCoords (float fSize)
{
  glPushMatrix();
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 0, 1, 0);
  DrawCubeFace (fSize);
  glRotatef (180, 0, 1, 0);
  DrawCubeFace (fSize);
  glPopMatrix();
}

/* Here goes our drawing code */
int drawFeedback( int BOX, float posx, float posy, float scale )
{
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );
	
	int W = m_viewport[2]-m_viewport[0];
	int H = m_viewport[3]-m_viewport[1];
	posx= (float)W/2+(float)W/2*posx;
	posy= (float)H/2+((float)H/2)*posy;
	
	printf("W: %d\n",W);
	printf("H: %d\n",H);
	printf("X: %f\n",posx);
	printf("Y: %f\n",posy);

	glPushMatrix();
	glLoadIdentity();
	glScalef(scale,scale,1);
	freetype::print(our_font, posx, posy, "%d", BOX);
	glPopMatrix();
    return( 1 );
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
	
	
	// Draw Objects in the list
    drawObject();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Disaplay update frequency 
	if (DRAWFPS)
		drawFPS();
	if (DRAWCLOCK){
		//Set Color to White
		glColor3f(1.0f,1.0f,1.0f); 
		drawCLOCK();
	}
	
	if (UDP.HAS_DATA()){
		feedback_box_count=120;
		feedback_box = std::atoi(UDP_BUFFER);
	}
	//Set Color to Red
	if (feedback_box_count>0){
		glColor3f(1,0,0); 
		drawFeedback(feedback_box,-.1,0,8);
		feedback_box_count--;
	}
    
	//Display frequency for each object
    if (DRAWFREQ) {
		//Set Color to Red
		glColor3f(1,0,0); 
		//Object loop
		for (unsigned int i=0;i<oList.size();i++){
			oList[i].printText();
		}
	}
	
	glPopMatrix();
	//Swap buffer to screen
    glutSwapBuffers ();
	glFlush();
	
}

//-------------------------------------------------------------------------
//  Draws the object
//-------------------------------------------------------------------------
void drawObject()
{
	layoutList.back().Draw();
}


//-------------------------------------------------------------------------
//  Draw FPS
//-------------------------------------------------------------------------
void drawFPS()
{
    //  Load the identity matrix so that FPS string being drawn
    //  won't get animates
    glLoadIdentity ();
    //  Print the FPS to the window
    printw (-0.9, -0.9, 0, (char*)"FPS: %4.2f", fps);
}

//-------------------------------------------------------------------------
//  Draw FPS
//-------------------------------------------------------------------------
void drawCLOCK()
{
	using std::chrono::system_clock;
	system_clock::time_point today = system_clock::now();
	std::time_t tt;
	tt = system_clock::to_time_t ( today );

	glPushMatrix();
 	glLoadIdentity ();
    //  Print the FPS to  the window
    printw (GLUT_BITMAP_HELVETICA_18, 0.7, 0.9, 0, (char*)"%s", ctime(&tt));
	glPopMatrix();
}

//-------------------------------------------------------------------------
//  This function is called when OpenGL\GLUT is not working on
//  something else... It is mainly used for animation...
//
//  It's like the timers but time intervals are dependent on how busy
//  the app is, instead of having a constant value set by the user.
//-------------------------------------------------------------------------
void idle (void)
{	
    //  Calculate FPS
    calculateFPS();
	
	//Verify UDP Buffer
	int rec = UDP.timed_recv(UDP_BUFFER,sizeof(UDP_BUFFER),1000);
	if (rec>0) {
		UDP_BUFFER[rec]=0;
		printf("T: %.*s\n",rec, UDP_BUFFER);
	}
	

    //  Call display function (draw the current frame)
    glutPostRedisplay ();

    glFlush();
    glFinish();
}

//-------------------------------------------------------------------------
// Calculates the frames per second
//-------------------------------------------------------------------------
void calculateFPS()
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;
    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);
        //  Set time
        previousTime = currentTime;
        //  Reset frame count
        frameCount = 0;
    }
}

//-------------------------------------------------------------------------
//  This function is passed to the glutReshapeFunc and is called
//  whenever the window is resized.
//-------------------------------------------------------------------------
void reshape (int w, int h)
{
	window_width = w;
    window_height = h;
	
	int scale = floor((h-SCREEN_SIZE_Y)/2);

    // Scale to the Screen Size
    glViewport(floor(w-SCREEN_SIZE_X-scale)/2, floor(h-SCREEN_SIZE_Y-scale)/2, SCREEN_SIZE_X+scale, SCREEN_SIZE_Y+scale);
}

extern GLvoid *font_style;
//-------------------------------------------------------------------------
//  This function is passed to the glutKeyboardFunc and is called
//  whenever a key is pressed.
//-------------------------------------------------------------------------
static void Key(unsigned char key, int x, int y)
{

    switch (key) {
		case '1':
			//changeLayout(0);
		break;
		case '2':
			//changeLayout(1);
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
    window_width = SCREEN_SIZE_X;
    window_height = SCREEN_SIZE_Y;

	// Define fonte style
    font_style = GLUT_BITMAP_9_BY_15;

    //  Connect to the windowing system
    glutInit(&argc, argv);

    //  create a window with the specified dimensions
    glutInitWindowSize (window_width, window_height);

    //  Set the window x and y coordinates such that the
    //  window becomes centered
    centerOnScreen ();

    //  Position Window
    glutInitWindowPosition (screenWindow_x, screenWindow_y);

    //  Set Display mode
    glutInitDisplayMode (GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_ACCUM);


    //  Create window with the specified title
    glutCreateWindow (window_title);

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
	
    //  Start GLUT event processing loop
    glutMainLoop();
	
}

#ifdef __linux__ 
bool InitVSync(){
	// Extension is supported, init pointers.
        glXSwapInterval = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");
/*        glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAYEXTPROC) glXGetProcAddress((const GLubyte *)"glXGetCurrentDisplayEXT");
        glXGetCurrentReadDrawable = (PFNGLXGETCURRENTDISPLAYEXTPROC) glXGetProcAddress((const GLubyte *)"glXGetCurrentReadDrawable");
*/

        
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
