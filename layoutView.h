#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <GL/glut.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "ssvepobject.h"
#include "FreeType.h"
#include "textObj.h"

extern int DRAWWITHPICTURES;
extern int DRAWFREQ;
extern int DRAWFPS;
extern int SCREEN_SIZE_X;
extern int SCREEN_SIZE_Y;


class layoutView
{
private:
	unsigned int _NBlocks;
	GLfloat BlockSize_X;
	GLfloat BlockSize_Y;
	GLfloat * _R;
    GLfloat * _Angles;
    float * _Freq;
	std::vector<std::string> _imageList;
	std::vector<std::string> _imageListN;

	std::string _Name;
	std::vector<std::string> _linksName;
	
	std::vector<textObj*> _textList;
		
    GLfloat color0[3];
    GLfloat color1[3];
	
	//List of SSVEP objects
	std::vector<SSVEPObject*> oList;
	
	std::string _Directory;
	std::string _Extension;
	int selectedFile;
	int maxFiles;
	float filesX;
	float filesY;
	std::vector<std::string> _listOfFiles;
	
	float freqBase[8] = {10,4.6,5.4,6.6,8.5,5,6,7.5};
	
public:
	layoutView(unsigned int NBlocks);
	~layoutView();
	
	
	void getPos(GLfloat * V2Pos, GLfloat R, GLfloat angle, GLfloat SizeX, GLfloat SizeY);
	
	void setAngles(const GLfloat *Angles);
	void setBlockSize(GLfloat X,GLfloat Y);
	void setR(const GLfloat *R);
	void setFrequencies(const float *Freq);
	
	void setImages(const char args[][25]);
	void setImagesN(const char args[][25]);
	
	void setName(const char args[25]);
	void setLinksName(const char args[][25]);
	std::string getLinkName(int Key);
	bool is(const char args[25]);
	
	std::string _linkTimedName;
	int _linkedTimer=0;
	void setLinksTimedName(int time, const char args[25]);
	
	
	void initObjects();
	
	void Draw(int slected, bool blink);
	
	void addText(std::string str, float X, float Y, float Scale,const GLfloat *color);
	
	void listUP();
	void listDOWN();
	void showListFrom(const char args[25], const char extension[5], int maxFiles, float X, float Y);
	freetype::font_data *listFont;
	
	freetype::font_data *our_font;
	
	
	int findSelected(int S);

};

#endif // LAYOUTVIEW_H
