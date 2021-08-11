/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <stdio.h>

#include <ofMain.h>
#include <ofxGui.h>

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
enum{
	BUF_SIZE_S = 500,
	BUF_SIZE_M = 1000,
	BUF_SIZE_L = 6000,
	BUF_SIZE_LL = 100000,
	UDP_BUF_SIZE = 100000,
};

enum{
	AUDIO_BUF_SIZE = 512,
	
	AUDIO_OUT_CHS = 2,
	AUDIO_IN_CHS = 2,
	AUDIO_BUFFERS = 2,			// 使われてないっぽい
	AUDIO_SAMPLERATE = 44100,
};

enum{
	NUM_HISTORY = 60,
};
	


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	
	ofxGuiGroup Group_Misc;
		ofxToggle b_depthTest;
		ofxToggle b_UseEasyCam;
		ofxToggle b_Separate_Disp_Send;
	
	ofxGuiGroup Group_Disp;
		ofxToggle b_DispImg;
		ofxToggle b_DispWebCam;
		ofxToggle b_DispFFT;
		ofxToggle b_DispArtSin;
		ofxToggle b_DispParticle;
		ofxToggle b_DispDanceCloud;
		
	ofxGuiGroup Group_Send;
		ofxToggle b_SendImg;
		ofxToggle b_SendWebCam;
		ofxToggle b_SendFFT;
		ofxToggle b_SendArtSin;
		ofxToggle b_SendParticle;
		ofxToggle b_SendDanceCloud;
	
	ofxGuiGroup Group_FFT;
		ofxFloatSlider Gain;
		ofxFloatSlider k_smooth;
		ofxFloatSlider dt_smooth_2;
		ofxToggle b_Window;
		
	ofxGuiGroup Group_WebCam;
		ofxFloatSlider col_WebCam;
		
	ofxGuiGroup Group_Img;
		ofxFloatSlider col_Img;
		
	ofxGuiGroup Group_DancingCloud;
		ofxFloatSlider bandRad;
		ofxFloatSlider bandVel;
		ofxToggle b_Limit_Rad_min;
		ofxColorSlider col_DancingCloud;
		
	ofxGuiGroup Group_ArtSin;
		ofxFloatSlider ArtSin_Band_min;
		ofxFloatSlider ArtSin_Band_max;
		ofxFloatSlider ArtSin_PhaseMap_k;
		ofxToggle b_ArtSin_abs;
		ofxToggle b_Window_artSin;
		ofxColorSlider col_ArtSin;
	
	ofxGuiGroup Group_Particle;
		ofxFloatSlider particleSize;
		ofxFloatSlider space_of_dot_x;
		ofxFloatSlider space_of_dot_y;
		ofxFloatSlider wave_Amp;
		ofxColorSlider col_particle;

	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

bool checkIf_ContentsExist(char* ret, char* buf);
void Align_StringOfData(string& s);
void print_separatoin();

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;


/************************************************************
************************************************************/

