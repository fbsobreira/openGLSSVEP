#include "layoutView.h"
#include <stdio.h>


#include <math.h>

layoutView::layoutView(int NBlocks)
{
	_R = (GLfloat*) malloc(NBlocks);
	_Angles = (GLfloat*) malloc(NBlocks);
	_Freq = (float*) malloc(NBlocks);
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

void layoutView::setAngles(const GLfloat *Angles){
	std::copy(Angles, Angles+_NBlocks, _Angles);
}
void layoutView::setBlockSize(GLfloat X,GLfloat Y){
	BlockSize_X=X;
	BlockSize_Y=Y;
}
void layoutView::setR(const GLfloat *R){
	std::copy(R, R+_NBlocks, _R);
}

void layoutView::setFrequencies(const float *Freq){
	std::copy(Freq, Freq+_NBlocks, _Freq);
}

void layoutView::initObjects(){
	GLfloat V2Pos[4];
	// Create blocks and add to list of objects
	for (unsigned int i=0;i<_NBlocks;i++){
		oList.push_back(SSVEPObject());
    }
	
	for (unsigned int i=0;i<_NBlocks;i++){
		//Genarate the position for each block 
		getPos(V2Pos,_R[i],_Angles[i],(GLfloat)(BlockSize_X),(GLfloat)(BlockSize_Y));
		//Apply values
		oList[i].setV2Pos(V2Pos);
		oList[i].setFrequency(_Freq[i]);
        oList[i].setColor(0,color0);
        oList[i].setColor(1,color1);
		//Load Image Name
		oList[i].img1 = _imageList[i];
		oList[i].img2 = _imageListN[i];
		
		//Iniciate Object
        oList[i].initObject();
    }

}

void layoutView::Draw(){
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
    for (unsigned int i=0;i<oList.size();i++){
		//Object display fucntion
		if (DRAWWITHPICTURES) {
			oList[i].DrawWithImage();
		} else {
			glCallList(oList[i].glDraw());
		}
    }
	glPopMatrix();
}
void layoutView::setImages(const char args[][25]){
	for (int i=0;i<_NBlocks;i++){
		_imageList.push_back(args[i]);
	}
}
void layoutView::setImagesN(const char args[][25]){
	for (int i=0;i<_NBlocks;i++){
		_imageListN.push_back(args[i]);
	}
}