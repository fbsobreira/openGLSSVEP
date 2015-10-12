#include <stdio.h>
#include <string>
#include <string.h>

#include "ssvepobject.h"
#include "glhelper.h"

SSVEPObject::SSVEPObject()
{
    glDrawC1 = glGenLists(1);
    glDrawC2 = glGenLists(1);

    FrameCount=0;

}
void SSVEPObject::setColor(int i, GLfloat *V){
    switch (i) {
    case 0:
        std::copy(V,V+3,color1);
        break;
    case 1:
        std::copy(V,V+3,color2);
        break;
    default:
        break;
    }
}

void SSVEPObject::setV2Pos(GLfloat *V){
    std::copy(V,V+4,V2Pos);
}

void SSVEPObject::initObject(){

    glNewList(glDrawC1, GL_COMPILE);
    glColor3f(color1[0],color1[1],color1[2]);
    glRectf(V2Pos[0],V2Pos[1],V2Pos[2],V2Pos[3]);
    glEndList();

    glNewList(glDrawC2, GL_COMPILE);
    glColor3f(color2[0],color2[1],color2[2]);
    glRectf(V2Pos[0],V2Pos[1],V2Pos[2],V2Pos[3]);
    glEndList();
}

void SSVEPObject::setFrequency(float freq){
    int freqInt = (int)(freq*10);
    switch (freqInt) {
    case 300:
        FrameCount1=1;
        FrameCount2=1;
        break;
    case 200:
        FrameCount1=2;
        FrameCount2=1;
        break;
    case 150:
        FrameCount1=2;
        FrameCount2=2;
        break;
    case 120:
        FrameCount1=3;
        FrameCount2=2;
        break;
    case 100:
        FrameCount1=3;
        FrameCount2=3;
        break;
    case 85:
        FrameCount1=4;
        FrameCount2=3;
        break;
    case 75:
        FrameCount1=4;
        FrameCount2=4;
        break;
    case 66:
        FrameCount1=5;
        FrameCount2=4;
        break;
    case 60:
        FrameCount1=5;
        FrameCount2=5;
        break;
    case 54:
        FrameCount1=6;
        FrameCount2=5;
        break;
    case 50:
        FrameCount1=6;
        FrameCount2=6;
        break;
    case 46:
        FrameCount1=7;
        FrameCount2=6;
        break;
    case 42:
        FrameCount1=7;
        FrameCount2=7;
        break;
    default:
        FrameCount1=1;
        FrameCount2=1;
        break;
    }
    FrameCount=0;
}

GLint SSVEPObject::glDraw(){
    int FreqAverage=1;
    int count = (LastColor==0)?FrameCount1:FrameCount2;

    if (++FrameCount>=count){
        LastColor = !LastColor;
        FrameCount=0;
        if (LastColor==0){
	    //if(++FreqCount>=FreqAverage){
            	int deltaTime = glutGet(GLUT_ELAPSED_TIME) - timeChanged;
            	timeChanged = glutGet(GLUT_ELAPSED_TIME);
	        Freq = 1000.0/deltaTime;
	        //Freq = (1000.0*FreqAverage/deltaTime);
		FreqCount=0;
	    //}
        }
    }
    if (LastColor==0) {
        return glDrawC1;
    }else {
        return glDrawC2;
    }
}

float SSVEPObject::Frequency(){
    return Freq;
}

void SSVEPObject::printText(){
    float v= (V2Pos[1]+V2Pos[3])/2;
    float h= (V2Pos[0]+V2Pos[2])/2;

    glLoadIdentity ();

    char buffer[50];
    sprintf(buffer,"Freq: %4.2fHz", Frequency());
    int strLen = strlen(buffer);

    printw (h-strLen*.014, v, 0, buffer);

}
