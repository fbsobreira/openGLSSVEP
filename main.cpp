
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
freetype::font_data our_fontBig;

int DRAWWITHPICTURES=1;
int DRAWFREQ=0;
int DRAWFPS=1;
int DRAWCLOCK=1;
//int SCREEN_SIZE_X=1920;
//int SCREEN_SIZE_Y=1080;
int SCREEN_SIZE_X=1600;
int SCREEN_SIZE_Y=900;

//UDP Buffer
char *UDP_BUFFER = new char[100];
// Start UDP Server
udp_server *UDP;
// feedback aux
#define FEEDBACK_TIMER 1.5*60
int feedback_box=0;
int feedback_box_count=0;
int selected = -1;


//Templete for Size
template<class T, size_t N>
size_t size(T (&)[N]) { return N; }

// Layouts List
std::vector<layoutView*> layoutList;
		
// current layout
unsigned int curr_layout = 0; 

bool calibration_mode = false;
int calibration_freq = 0;


void setLayout(std::string Name){
	for (unsigned int i=0;i<layoutList.size();i++){
		if (layoutList[i]->is(Name.c_str()))
			curr_layout=i;
	}
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
	
	our_font.init("Test.ttf", 16);
	our_fontBig.init("Test.ttf", 30);
	
	//Create Layout 0 
	//Nurse Call
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Nurse"});
	layoutList.back()->setLinksName((const char[][25]){{"0"},{"index"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"NurseButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"NurseButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	
	//Sleep Mode
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"SleepMode"});
	layoutList.back()->setLinksName((const char[][25]){{"0"},{"index"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"SleepModeButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"SleepModeButton.png"}});
	
	
	//Create Layout 1
	layoutList.push_back(new layoutView(8));
	layoutList.back()->setName((const char[25]){"index"});
	layoutList.back()->setLinksName((const char[][25]){{"Nurse"},{"Speller"},
		{"Yes"},{"CommonNeeds"},{"SleepMode"},{"MediaControl"},
		{"No"},{"MedicalNeeds"}});
	layoutList.back()->setAngles((const GLfloat[]){0,38,90,142,180,218,270,322});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.7,0.75,0.8,0.75,0.7,0.75,0.8,0.75});
	layoutList.back()->setFrequencies((const float []){10,4.6,5.4,6.6,8.5,5,6,7.5});
	//layoutList.back()->setFrequencies((const float []){4.2,4.6,5,5.4,6,6.6,7.5,8.5});
	layoutList.back()->setImages((const char[][25]){{"NurseButton.png"},{"SpellerButton.png"},
		{"YesButton.png"},{"CommonNeedsButton.png"},{"SleepModeButton.png"},{"MediaControlButton.png"},
		{"NoButton.png"},{"MedicalNeedsButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"NurseButtonN.png"},{"SpellerButtonN.png"},
		{"YesButtonN.png"},{"CommonNeedsButtonN.png"},{"SleepModeButtonN.png"},{"MediaControlButtonN.png"},
		{"NoButtonN.png"},{"MedicalNeedsButtonN.png"}});
		
	
	// Init objects
	for (unsigned int i=0;i<layoutList.size();i++)
		layoutList[i]->initObjects();
	
}

/* Here goes our drawing code */
int drawFeedback( int BOX, float posx, float posy, float scale )
{
	glPushMatrix();
	glLoadIdentity();
	glScalef(scale,scale,1);
	freetype::print(our_fontBig, posx, posy, "%d", BOX);
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
	if (calibration_mode)
		drawObject(calibration_freq, true);
	else
		drawObject(selected, false); 
	//Clear texture to write text
	glBindTexture(GL_TEXTURE_2D, 0);
	// Disaplay update frequency 
	if (DRAWFPS){
		//Set Color to Red
		glColor3f(1.0f,0.0f,0.0f); 
		drawFPS();
	}
	if (DRAWCLOCK){
		//Set Color to White
		glColor3f(1.0f,1.0f,1.0f); 
		drawCLOCK();
	}	
	if (feedback_box_count>0){
		//Set Color to Red
		glColor3f(1.0f,0.0f,0.0f); 
		drawFeedback(feedback_box,-.1,0.0,3);
		feedback_box_count--;
		if (feedback_box_count==0){
			setLayout( layoutList[curr_layout]->getLinkName(selected));
			selected = -1;
		}
	}
    
	//Display frequency for each object
    if (DRAWFREQ) {
		//Set Color to Red
		glColor3f(1.0f,0.0f,0.0f); 
	}
	
	glPopMatrix();
	//Swap buffer to screen
    glutSwapBuffers ();
	glFlush();
	
}

//-------------------------------------------------------------------------
//  Draws the object
//-------------------------------------------------------------------------
void drawObject(int selected, bool blink)
{
	layoutList[curr_layout]->Draw(selected, blink);
}

//-------------------------------------------------------------------------
//  Draw FPS
//-------------------------------------------------------------------------
void drawFPS()
{
   	glPushMatrix();
 	glLoadIdentity ();
	glScalef(.5,.5,1);
    //  Print the FPS to the window
	freetype::print(our_font, -0.9, -0.9, "FPS: %4.2f", fps);
	glPopMatrix();
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
	glScalef(.7,.7,1);
    //  Print the FPS to  the window
	//printw (GLUT_BITMAP_HELVETICA_18, 0.7, 0.9, 0, (char*)"%s", ctime(&tt));
    //printw (GLUT_BITMAP_TIMES_ROMAN_24, 0.7, 0.9, 0, (char*)"%s", ctime(&tt));
	//printw (GLUT_BITMAP_9_BY_15, 0.7, 0.8, 0, (char*)"%s", ctime(&tt));
	freetype::print(our_font, 0.65, 0.9, "%s", ctime(&tt));
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
	int rec = UDP->timed_recv(UDP_BUFFER,100,100);
	if (rec>0) {
		//UDP_BUFFER[rec]=0;
		printf("T:%d: %.*s\n",rec,rec, UDP_BUFFER);
	}
	if (UDP->HAS_DATA()){
		exec(UDP_BUFFER[0]);
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
	
	int scaleX = floor((w-SCREEN_SIZE_X)/2);
	int scaleY = floor((h-SCREEN_SIZE_Y)/2);

    // Scale to the Screen Size
    glViewport(floor(w-SCREEN_SIZE_X-scaleX)/2, floor(h-SCREEN_SIZE_Y-scaleY)/2, SCREEN_SIZE_X+scaleX, SCREEN_SIZE_Y+scaleY);
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
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			exec(key);
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
		//Training Mode
		case 'C':
			exec('C');
		break;
		case 'R':
			exec('R');
		break;
		case 27:
			//glutLeaveMainLoop();
			glutExit();
			//exit(0);
    }
	// RE-Display
    glutPostRedisplay();
}


void exec(char code){
	if (code=='C') {
		calibration_mode = true;
	}else if (code=='R') {
		calibration_mode = false;
	}else {
		if (calibration_mode){
			code = std::atoi(&code);
			calibration_freq = code-1;
		}
		else {
			code = std::atoi(&code);
			feedback_box_count=FEEDBACK_TIMER;
			feedback_box = code;
			selected = feedback_box-1;
		}
	}
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
	
	//Init UDP
	UDP = new udp_server("127.0.0.1", 1050);
	
	setLayout("index");
	
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
