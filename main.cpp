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
#include "udp_client.h" 
#include <iostream>          // For cout and cerr
#include <cstdlib>           // For atoi()
#include <unistd.h>

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

int screenWindow_x;
int screenWindow_y;

//  variables representing the window size
int window_width;
int window_height;


//UDP Buffer
char *UDP_BUFFER = new char[100];
// Start UDP Server
udp_server *UDP;
udp_client *UDPClient;
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
		
	
	//Speller
	layoutList.push_back(new layoutView(8));
	layoutList.back()->setName((const char[25]){"Speller"});
	layoutList.back()->setLinksName((const char[][25]){{"9-@"},{"backspace"},
		{"index"},{"a-g"},{"h-n"},{"o-u"},
		{"v-1"},{"2-8"}});
	layoutList.back()->setAngles((const GLfloat[]){0,38,90,142,180,218,270,322});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.7,0.75,0.8,0.75,0.7,0.75,0.8,0.75});
	layoutList.back()->setFrequencies((const float []){10,4.6,5.4,6.6,8.5,5,6,7.5});
	layoutList.back()->setImages((const char[][25]){{"9-@Button.png"},{"BackspaceButon.png"},
		{"BackButton.png"},{"A-GButton.png"},{"H-NButton.png"},{"O-UButton.png"},
		{"V-1Button.png"},{"2-8Button.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"9-@ButtonN.png"},{"BackspaceButonN.png"},
		{"BackButtonN.png"},{"A-GButtonN.png"},{"H-NButtonN.png"},{"O-UButtonN.png"},
		{"V-1ButtonN.png"},{"2-8ButtonN.png"}});
		
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
	
	//Common Needs
	layoutList.push_back(new layoutView(8));
	layoutList.back()->setName((const char[25]){"CommonNeeds"});
	layoutList.back()->setLinksName((const char[][25]){{"Hunger"},{"Template"},
		{"index"},{"Bedding"},{"RoomBrightness"},{"Bathroom"},
		{"FreshAir"},{"Thirst"}});
	layoutList.back()->setAngles((const GLfloat[]){0,38,90,142,180,218,270,322});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.7,0.75,0.8,0.75,0.7,0.75,0.8,0.75});
	layoutList.back()->setFrequencies((const float []){10,4.6,5.4,6.6,8.5,5,6,7.5});
	layoutList.back()->setImages((const char[][25]){{"HungerButton.png"},{"TemplateButton.png"},
		{"BackButton.png"},{"BeddingButton.png"},{"RoomBriButton.png"},{"BathroomButton.png"},
		{"FreshAirButton.png"},{"ThirstButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"HungerButtonN.png"},{"TemplateButtonN.png"},
		{"BackButtonN.png"},{"BeddingButtonN.png"},{"RoomBriButtonN.png"},{"BathroomButtonN.png"},
		{"FreshAirButtonN.png"},{"ThirstButtonN.png"}});
		
	
	//Thirst Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Thirst"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"CommonNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"ThirstButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"ThirstButtons.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//FreshAir Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"FreshAir"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"CommonNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"FreshAirButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"FreshAirButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Bathroom Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Bathroom"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"CommonNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"BathroomButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"BathroomButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//RoomBrightness Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"RoomBrightness"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"CommonNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"RoomBriButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"RoomBriButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Bedding Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Bedding"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"CommonNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"BeddingButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"BeddingButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Hunger Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Hunger"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"CommonNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"HungerButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"HungerButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
		
	//Medical Needs
	layoutList.push_back(new layoutView(8));
	layoutList.back()->setName((const char[25]){"MedicalNeeds"});
	layoutList.back()->setLinksName((const char[][25]){{"Uncomfortable"},{"Itching"},
		{"index"},{"BreathingProblems"},{"Pain"},{"Template"},
		{"SalivaProblems"},{"Headache"}});
	layoutList.back()->setAngles((const GLfloat[]){0,38,90,142,180,218,270,322});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.7,0.75,0.8,0.75,0.7,0.75,0.8,0.75});
	layoutList.back()->setFrequencies((const float []){10,4.6,5.4,6.6,8.5,5,6,7.5});
	layoutList.back()->setImages((const char[][25]){{"UncomfortableButton.png"},{"ItchingButton.png"},
		{"BackButton.png"},{"BreathingProButton.png"},{"PainButton.png"},{"TemplateButton.png"},
		{"SalivaProButton.png"},{"HeadacheButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"UncomfortableButtonN.png"},{"ItchingButtonN.png"},
		{"BackButtonN.png"},{"BreathingProButtonN.png"},{"PainButtonN.png"},{"TemplateButtonN.png"},
		{"SalivaProButtonN.png"},{"HeadacheButtonN.png"}});
	
	
	//Headache Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Headache"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"MedicalNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"HeadacheButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"HeadacheButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;

	//SalivaProblems Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"SalivaProblems"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"MedicalNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"SalivaProButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"SalivaProButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Pain Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Pain"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"MedicalNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"PainButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"PainButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//BreathingProblems Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"BreathingProblems"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"MedicalNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"BreathingProButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"BreathingProButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Itching Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Itching"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"MedicalNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"ItchingButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"ItchingButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Uncomfortable Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"Uncomfortable"});
	layoutList.back()->setLinksName((const char[][25]){{"index"},{"MedicalNeeds"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"UncomfortableButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"UncomfortableButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	//Sleep Mode Confirm
	layoutList.push_back(new layoutView(3));
	layoutList.back()->setName((const char[25]){"SleepMode"});
	layoutList.back()->setLinksName((const char[][25]){{"SleepModeIn"},{"index"},{"0"}});
	layoutList.back()->setAngles((const GLfloat[]){90,270,0});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,0.8,0});
	layoutList.back()->setFrequencies((const float []){5.4,6,0});
	layoutList.back()->setImages((const char[][25]){{"YesButton.png"},{"NoButton.png"},{"SleepModeButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"YesButtonN.png"},{"NoButtonN.png"},{"SleepModeButton.png"}});
	layoutList.back()->addText((std::string)"You have selected:",-.28,0.3,1,(const GLfloat[]){1,0,0});
	layoutList.back()->addText((std::string)"Is that correct?",-.22,-0.35,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	
	
	//Sleep Mode
	layoutList.push_back(new layoutView(1));
	layoutList.back()->setName((const char[25]){"SleepModeIn"});
	layoutList.back()->setLinksName((const char[][25]){{"index"}});
	layoutList.back()->setAngles((const GLfloat[]){180});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.8,});
	layoutList.back()->setFrequencies((const float []){6});
	layoutList.back()->setImages((const char[][25]){{"SleepModeButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"SleepModeButtonN.png"}});
	
	
	//Media Control
	layoutList.push_back(new layoutView(4));
	layoutList.back()->setName((const char[25]){"MediaControl"});
	layoutList.back()->setLinksName((const char[][25]){{"_ListUP"},{"index"},
		{"_ListDOWN"},{"Select"}});
	layoutList.back()->setAngles((const GLfloat[]){0,90,180,270});
	layoutList.back()->setBlockSize((GLfloat)0.3,(GLfloat)(0.45));
	layoutList.back()->setR((const GLfloat[]){0.7,0.8,0.7,0.8});
	layoutList.back()->setFrequencies((const float []){4.6,5.4,6.6,8.5});
	layoutList.back()->setImages((const char[][25]){{"UpButton.png"},{"BackButton.png"},
		{"DownButton.png"},{"SelectButton.png"}});
	layoutList.back()->setImagesN((const char[][25]){{"UpButtonN.png"},{"BackButtonN.png"},
		{"DownButtonN.png"},{"SelectButtonN.png"}});
	layoutList.back()->addText((std::string)"Media Control",-.7,0.4,1,(const GLfloat[]){1,0,0});
	layoutList.back()->our_font = &our_fontBig;
	layoutList.back()->showListFrom((const char[25]){"media/movies/"},(const char[5]){".avi"},10,-.2,.2);
	layoutList.back()->listFont = &our_font;
	
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
	glutSetWindow( WindowID1 );

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
		case 'T':
			UDPClient->send("Wellcome",8);
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
		setLayout("index");
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
			if (layoutList[curr_layout]->is("MediaControl")){	
				selected = code-1;
				if (strcmp(layoutList[curr_layout]->getLinkName(selected).c_str(),"_ListUP")==0){
					layoutList[curr_layout]->listUP();
					selected=-1;
					return;
				}else if (strcmp(layoutList[curr_layout]->getLinkName(selected).c_str(),"_ListDOWN")==0){
					layoutList[curr_layout]->listDOWN();
					selected=-1;
					return;
				}
			}
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
    WindowID1=glutCreateWindow (window_title);

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
	UDP = new udp_server("0.0.0.0", 1050);
	UDPClient = new udp_client("127.0.0.1", 1051);
	
	setLayout("index");
	//setLayout("MediaControl");
	
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
