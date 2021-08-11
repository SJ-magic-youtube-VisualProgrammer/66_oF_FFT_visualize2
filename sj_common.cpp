/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/
/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double sj_max(double a, double b)
{
	if(a < b)	return b;
	else		return a;
}

/******************************
******************************/
bool checkIf_ContentsExist(char* ret, char* buf)
{
	if( (ret == NULL) || (buf == NULL)) return false;
	
	string str_Line = buf;
	Align_StringOfData(str_Line);
	vector<string> str_vals = ofSplitString(str_Line, ",");
	if( (str_vals.size() == 0) || (str_vals[0] == "") ){ // no_data or exist text but it's",,,,,,,".
		return false;
	}else{
		return true;
	}
}

/******************************
******************************/
void Align_StringOfData(string& s)
{
	size_t pos;
	while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/******************************
******************************/
void print_separatoin()
{
	printf("---------------------------------\n");
}

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_Misc.setup("Misc");
		Group_Misc.add(b_depthTest.setup("b_depthTest", false));
		Group_Misc.add(b_UseEasyCam.setup("b_UseEasyCam", true));
		Group_Misc.add(b_Separate_Disp_Send.setup("b_Separate_Disp_Send", true));
	gui.add(&Group_Misc);
	
	Group_Disp.setup("Disp");
		Group_Disp.add(b_DispImg.setup("b_DispImg", true));
		Group_Disp.add(b_DispWebCam.setup("b_DispWebCam", false));
		Group_Disp.add(b_DispFFT.setup("b_DispFFT", true));
		Group_Disp.add(b_DispArtSin.setup("b_DispArtSin", true));
		Group_Disp.add(b_DispParticle.setup("b_DispParticle", true));
		Group_Disp.add(b_DispDanceCloud.setup("b_DispDanceCloud", true));
	gui.add(&Group_Disp);
	
	Group_Send.setup("Send");
		Group_Send.add(b_SendImg.setup("b_SendImg", true));
		Group_Send.add(b_SendWebCam.setup("b_SendWebCam", false));
		Group_Send.add(b_SendFFT.setup("b_SendFFT", true));
		Group_Send.add(b_SendArtSin.setup("b_SendArtSin", true));
		Group_Send.add(b_SendParticle.setup("b_SendParticle", true));
		Group_Send.add(b_SendDanceCloud.setup("b_SendDanceCloud", true));
	gui.add(&Group_Send);
		
	Group_Img.setup("Img");
		Group_Img.add(col_Img.setup("col_Img", 50.0, 0.0, 255.0));
	gui.add(&Group_Img);
	
	Group_WebCam.setup("WebCam");
		Group_WebCam.add(col_WebCam.setup("col_WebCam", 50.0, 0.0, 255.0));
	gui.add(&Group_WebCam);
	
	Group_FFT.setup("FFT");
		Group_FFT.add(Gain.setup("Gain", 1.0, 1.0, 5.0));
		Group_FFT.add(k_smooth.setup("k_smooth", 0.95, 0.8, 1.0));
		Group_FFT.add(dt_smooth_2.setup("dt_smooth_2", 167, 10, 300));
		Group_FFT.add(b_Window.setup("b_window", true));
	gui.add(&Group_FFT);
	
	Group_DancingCloud.setup("DancingCloud");
		Group_DancingCloud.add(bandRad.setup("bandRad", 2.0, 1.0, 10.0));
		Group_DancingCloud.add(bandVel.setup("bandVel", 100.0, 1.0, 255.0));
		Group_DancingCloud.add(b_Limit_Rad_min.setup("b_Limit_Rad_min", true));

		{
			ofColor initColor = ofColor(10, 100, 255);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_DancingCloud.add(col_DancingCloud.setup("col_DancingCloud", initColor, minColor, maxColor));
		}
	gui.add(&Group_DancingCloud);
	
	Group_ArtSin.setup("ArtSin");
		Group_ArtSin.add(ArtSin_Band_min.setup("ArtSin_Band_min", 1.0, 1.0, 255.0));
		Group_ArtSin.add(ArtSin_Band_max.setup("ArtSin_Band_max", 1.0, 1.0, 255.0));
		Group_ArtSin.add(ArtSin_PhaseMap_k.setup("ArtSin_PhaseMap_k", 1.0, 0.0, 2.0));
		Group_ArtSin.add(b_ArtSin_abs.setup("b_ArtSin_abs", false));
		Group_ArtSin.add(b_Window_artSin.setup("b_Window_artSin", false));
		{
			ofColor initColor = ofColor(0, 100, 255, 150);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_ArtSin.add(col_ArtSin.setup("col_ArtSin", initColor, minColor, maxColor));
		}
	gui.add(&Group_ArtSin);
	
	Group_Particle.setup("Particle");
		Group_Particle.add(particleSize.setup("particleSize", 2.0, 1.0, 10.0));
		Group_Particle.add(space_of_dot_x.setup("space_of_dot_x", 1.0, 1.0, 20));
		Group_Particle.add(space_of_dot_y.setup("space_of_dot_y", 5.0, 1.0, 20));
		Group_Particle.add(wave_Amp.setup("wave_Amp", 50.0, 0.0, 100.0));
		
		{
			ofColor initColor = ofColor(50, 127, 255, 255);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_Particle.add(col_particle.setup("col_particle", initColor, minColor, maxColor));
		}
	gui.add(&Group_Particle);

	/********************
	********************/
	gui.minimizeAll();
}

