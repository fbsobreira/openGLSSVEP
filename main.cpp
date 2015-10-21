
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
#include <math.h>

#include "ssvepobject.h"
#include "glhelper.h"

#include "main.h"

//GLint thing1, thing2;
std::vector<SSVEPObject> oList;

void getPos(GLfloat * V2Pos, GLfloat R, GLfloat angle, GLfloat Size){
    angle = angle * 3.14 / 180.0 ;
    V2Pos[0] = R*(GLfloat)sin(angle)-Size/2;
    V2Pos[1] = R*(GLfloat)cos(angle)+Size/2;
    V2Pos[2] = R*(GLfloat)sin(angle)+Size/2;
    V2Pos[3] = R*(GLfloat)cos(angle)-Size/2;
    return;
}

//-------------------------------------------------------------------------
//  Set OpenGL program initial state.
//-------------------------------------------------------------------------
void init ()
{
    //  Set the frame buffer clear color to black.
    glClearColor (0.0, 0.0, 0.0, 1.0);
    GLfloat R=0.7;
    GLfloat Size=0.35;
    GLfloat Angle[] = {0,45,90,135,180,225,270,315};
    float freq[] = {10,4.6,5.4,6.6,8.5,5,6,7.5};

    GLfloat color0[3] = {1.0, 1.0, 1.0};
    GLfloat color1[3] = {0.0, 0.0, 0.0};

    GLfloat V2Pos1[4];// = {-0.9, 0.8, -0.6, 0.2};
    getPos(V2Pos1,R,Angle[0],Size);
    oList.push_back(SSVEPObject());
    oList[0].setV2Pos(V2Pos1);
    oList[0].setFrequency(freq[0]);

    GLfloat V2Pos2[4]; // = {-0.4, 0.8, -0.1, 0.2};
    getPos(V2Pos2,R,Angle[1],Size);
    oList.push_back(SSVEPObject());
    oList[1].setV2Pos(V2Pos2);
    oList[1].setFrequency(freq[1]);

    GLfloat V2Pos3[4] = {0.1, 0.8, 0.4, 0.2};
    getPos(V2Pos3,R,Angle[2],Size);
    oList.push_back(SSVEPObject());
    oList[2].setV2Pos(V2Pos3);
    oList[2].setFrequency(freq[2]);

    GLfloat V2Pos4[4] = {0.6, 0.8, 0.9, 0.2};
    getPos(V2Pos4,R,Angle[3],Size);
    oList.push_back(SSVEPObject());
    oList[3].setV2Pos(V2Pos4);
    oList[3].setFrequency(freq[3]);


    GLfloat V2Pos5[4] = {-0.9, -0.8, -0.6, -0.2};
    getPos(V2Pos5,R,Angle[4],Size);
    oList.push_back(SSVEPObject());
    oList[4].setV2Pos(V2Pos5);
    oList[4].setFrequency(freq[4]);

    GLfloat V2Pos6[4] = {-0.4, -0.8, -0.1, -0.2};
    getPos(V2Pos6,R,Angle[5],Size);
    oList.push_back(SSVEPObject());
    oList[5].setV2Pos(V2Pos6);
    oList[5].setFrequency(freq[5]);

    GLfloat V2Pos7[4] = {0.1, -0.8, 0.4, -0.2};
    getPos(V2Pos7,R,Angle[6],Size);
    oList.push_back(SSVEPObject());
    oList[6].setV2Pos(V2Pos7);
    oList[6].setFrequency(freq[6]);

    GLfloat V2Pos8[4] = {0.6, -0.8, 0.9, -0.2};
    getPos(V2Pos8,R,Angle[7],Size);
    oList.push_back(SSVEPObject());
    oList[7].setV2Pos(V2Pos8);
    oList[7].setFrequency(freq[7]);

    for (unsigned int i=0;i<oList.size();i++){
        oList[i].setColor(0,color0);
        oList[i].setColor(1,color1);
        oList[i].initObject();
    }
}

//-------------------------------------------------------------------------
//  This function is passed to glutDisplayFunc in order to display
//	OpenGL contents on the window.
//-------------------------------------------------------------------------
void display (void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    drawObject();
    drawFPS();

    //Text
    glColor3f(1,0,0);
    for (unsigned int i=0;i<oList.size();i++){
        oList[i].printText();
    }
    glutSwapBuffers ();

    //glFlush();
    //glFinish();
}

//-------------------------------------------------------------------------
//  Draws the object
//-------------------------------------------------------------------------
void drawObject()
{
    //glScalef(0.8, 0.8, 1.0);
    for (unsigned int i=0;i<oList.size();i++){
        glCallList(oList[i].glDraw());
    }
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
//  This function is called when OpenGL\GLUT is not working on
//  something else... It is mainly used for animation...
//
//  It's like the timers but time intervals are dependent on how busy
//  the app is, instead of having a constant value set by the user.
//-------------------------------------------------------------------------
void idle (void)
{
    //  Animate the object
    //animateObject();

    //  Calculate FPS
    calculateFPS();

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
    //  Stay updated with the window width and height
    window_width = w;
    window_height = h;

    //  Reset viewport
    glViewport(0, 0, window_width, window_height);
}

extern GLvoid *font_style;

//-------------------------------------------------------------------------
//  Program Main method.
//-------------------------------------------------------------------------
int main (int argc, char **argv)
{

    //  variables representing the window size
    window_width = 640;
    window_height = 480;

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
    glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_ACCUM);


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
    glutIdleFunc (idle);

    //  Start GLUT event processing loop
    glutMainLoop();
}

