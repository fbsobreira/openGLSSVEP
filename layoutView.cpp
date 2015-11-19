#include "layoutView.h"
#include <stdio.h>
#include <dirent.h>
#include <algorithm>
#include <math.h>

// Initiate layout memory
layoutView::layoutView(unsigned int NBlocks)
{
	_R = (GLfloat*) malloc(NBlocks * sizeof(GLfloat));
	_Angles = (GLfloat*) malloc(NBlocks * sizeof(GLfloat));
	_Freq = (float*) malloc(NBlocks * sizeof(float));
	_NBlocks = NBlocks;
}

layoutView::~layoutView()
{
}

//Generate Positon based on Polar Coordinates
void layoutView::getPos(GLfloat * V2Pos, GLfloat R, GLfloat angle, GLfloat SizeX, GLfloat SizeY){
    angle = angle * 3.14 / 180.0 ;
    V2Pos[0] = R*(GLfloat)sin(angle)-SizeX/2;
    V2Pos[1] = R*(GLfloat)cos(angle)+SizeY/2;
    V2Pos[2] = R*(GLfloat)sin(angle)+SizeX/2;
    V2Pos[3] = R*(GLfloat)cos(angle)-SizeY/2;
	
	for (int i=0;i<4;i++){
		if (V2Pos[i]>1) V2Pos[i]=1;
		if (V2Pos[i]<-1) V2Pos[i]=-1;
	}
	
    return;
}
// Name and Links
void layoutView::setName(const char args[25]){
	_Name = std::string(args);
	//std::copy(Angles, Angles+_NBlocks, _Angles);
}
void layoutView::setLinksName(const char args[][25]){
	for (unsigned int i=0;i<_NBlocks;i++){
		_linksName.push_back(args[i]);
	}
}
std::string layoutView::getLinkName(int Key){
	if ((Key>=0) and (Key<_NBlocks))
		return _linksName[Key];
	else return std::string("");
}
bool layoutView::is(const char args[25]){
	return (_Name.compare(std::string(args)) == 0);
	//std::copy(Angles, Angles+_NBlocks, _Angles);
}
//
void layoutView::addText(std::string str, GLfloat X, GLfloat Y, GLfloat Scale,const GLfloat *color){
	_textList.push_back(new textObj());
	_textList.back()->_Text = str;
	_textList.back()->_X = X;
	_textList.back()->_Y = Y;
	_textList.back()->_Scale = Scale;
	std::copy(color,color + 2, _textList.back()->_Color);
}



// Copy Angles to Object
void layoutView::setAngles(const GLfloat *Angles){
	std::copy(Angles, Angles+_NBlocks, _Angles);
}
// Set block size
void layoutView::setBlockSize(GLfloat X,GLfloat Y){
	BlockSize_X=X;
	BlockSize_Y=Y;
}
// Copy Rs to Object
void layoutView::setR(const GLfloat *R){
	std::copy(R, R+_NBlocks, _R);
}
// Copy Frequencies to Object
void layoutView::setFrequencies(const float *Freq){
	std::copy(Freq, Freq+_NBlocks, _Freq);
}
// Initiate objects position
void layoutView::initObjects(){
	GLfloat V2Pos[4];
	// Create blocks and add to list of objects
	if (_NBlocks>8)_NBlocks=8;
	for (unsigned int i=0;i<_NBlocks;i++){
		oList.push_back(new SSVEPObject());
    }
	
	for (unsigned int i=0;i<_NBlocks;i++){
		//Genarate the position for each block 
		getPos(V2Pos,_R[i],_Angles[i],(GLfloat)(BlockSize_X),(GLfloat)(BlockSize_Y));
		//Apply values
		oList[i]->setV2Pos(V2Pos);
		oList[i]->setFrequency(_Freq[i]);
        oList[i]->setColor(0,color0);
        oList[i]->setColor(1,color1);
		//Load Image Name
		oList[i]->img1 = _imageList[i];
		oList[i]->img2 = _imageListN[i];
		
		//Iniciate Object
        oList[i]->initObject();
    }
	
	if (_Directory.length()>0){
		selectedFile = 0;
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir (_Directory.c_str())) != NULL) {
		  /* print all the files and directories within directory */
		  while ((ent = readdir (dir)) != NULL) {
			  char *point;
			  if((point = strrchr(ent->d_name,'.')) != NULL ) {
				 if(strcmp(point,_Extension.c_str()) == 0) {
					_listOfFiles.push_back(ent->d_name);
				  }
			  }
		  }
		  sort(_listOfFiles.begin(), _listOfFiles.end());
		  closedir (dir);
		}
	}
}
// Draw object to screen
void layoutView::Draw(int selected, bool blink){
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	if ((selected>=0) && (selected<oList.size())) {
		if (DRAWWITHPICTURES) {
			oList[selected]->DrawWithImage(blink);
		} else {
			glCallList(oList[selected]->glDraw());
		}
	}else{
		for (unsigned int i=0;i<oList.size();i++){
			//Object display fucntion
			if (DRAWWITHPICTURES) {
				oList[i]->DrawWithImage(true);
			} else {
				glCallList(oList[i]->glDraw());
			}
		}
	}
	
	for (unsigned int i=0;i<_textList.size();i++){
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0f,0.0f,0.0f);
		glScalef(_textList[i]->_Scale,_textList[i]->_Scale,1);
		freetype::print(*our_font, _textList[i]->_X, _textList[i]->_Y, "%s", _textList[i]->_Text.c_str());
		glPopMatrix();
	}
	
	//List Files
	glPushMatrix();
	glLoadIdentity();
	int listPos=0;
	if (selectedFile>=maxFiles){
		listPos = selectedFile-maxFiles+1;
	}
	int listEnd = _listOfFiles.size();
	if (listEnd>maxFiles) {
		if (selectedFile>=maxFiles){
			listEnd = selectedFile-1;
		}
	}
	for (unsigned int i=listPos;i<listEnd;i++){
		if (i==selectedFile)
			glColor3f(1.0f,0.0f,0.0f);
		else
			glColor3f(1.0f,1.0f,1.0f);
		
		float posY = filesY-freetype::fontSize(*listFont)*(i-listPos);
		glScalef(1,1,1);
		freetype::print(*listFont, filesX, posY, "%s", _listOfFiles[i].c_str());
	}
	glPopMatrix();
	
	
	glPopMatrix();
}
// Set Images 
void layoutView::setImages(const char args[][25]){
	for (unsigned int i=0;i<_NBlocks;i++){
		_imageList.push_back(args[i]);
	}
}
// Set Negative Images 
void layoutView::setImagesN(const char args[][25]){
	for (unsigned int i=0;i<_NBlocks;i++){
		_imageListN.push_back(args[i]);
	}
}

void layoutView::showListFrom(const char args[25], const char extension[5], int maxFiles, float X, float Y){
	_Directory = std::string(args);
	_Extension = std::string(extension);
	
	this->maxFiles = maxFiles;
	filesX = X;
	filesY = Y;
}
void layoutView::listUP(){
	if (selectedFile>0) selectedFile--;
}
void layoutView::listDOWN(){
	if (selectedFile<(_listOfFiles.size()-1))
		selectedFile ++;
}