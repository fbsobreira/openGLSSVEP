#ifndef SSVEPOBJECT_H
#define SSVEPOBJECT_H

#include <GL/glut.h>

class SSVEPObject
{
private:

    GLfloat color1[3];
    GLfloat color2[3];
    GLfloat V2Pos[4];

    GLint glDrawC1;
    GLint glDrawC2;

    int LastColor;
    int FrameCount;
    int FrameCount1;
    int FrameCount2;
    float Freq;

    int timeChanged;
    int FreqCount;

public:
    SSVEPObject();

    void setFrequency(float freq);

    float Frequency();

    void setColor(int i, float *V);
    void setV2Pos(float *V);

    void initObject();
    GLint glDraw();
    void printText();

//signals:

//public slots:
};

#endif // SSVEPOBJECT_H
