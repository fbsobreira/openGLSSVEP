#ifndef __GLHELPER_H_
#define __GLHELPER_H_

//  define the window position on screen
extern int screenWindow_x;
extern int screenWindow_y;

//  variables representing the window size
extern int window_width;
extern int window_height;

//  Support Functions
void centerOnScreen ();

//  printf prints to file. printw prints to window
void printw (float x, float y, float z, char* format, ...);


#endif // GLHELPER
