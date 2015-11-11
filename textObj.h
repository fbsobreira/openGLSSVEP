#ifndef TEXTOBJ_H
#define TEXTOBJ_H

#include <GL/glut.h>
#include <string>

class textObj
{
public:
	std::string _Text;
	GLfloat _X;
	GLfloat _Y;
	GLfloat _Scale;
	GLfloat _Color[3];

	textObj();
	~textObj();

};

#endif // TEXTOBJ_H
