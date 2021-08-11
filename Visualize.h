/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "fft.h"
#include <ofxVboSet.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class VISUALIZE{
private:
	/****************************************
	****************************************/
	enum{
		NUM_CLOUD_POINTS	= 400,
	};
	
	enum{
		PARTICLE_WIDTH	= AUDIO_BUF_SIZE,
		PARTICLE_HEIGHT	= NUM_HISTORY,
		
		// NUM_PARTICLES	= PARTICLE_WIDTH * PARTICLE_HEIGHT * 2, // GL_LINES
		NUM_PARTICLES	= PARTICLE_WIDTH * PARTICLE_HEIGHT,
	};
	ofEasyCam EasyCam;
	
	ofx__VBO_SET VboSet__Particle;
	ofx__VBO_SET VboSet__ArtSin;
	ofx__VBO_SET VboSet__FFT;
	ofColor LastCol_Particle;
	ofColor LastCol_ArtSin;
	
	
	float Rad = 500;		//Cloud raduis parameter
	float Vel = 0.1;		//Cloud points velocity parameter
	int bandRad = 2;		//Band index in spectrum, affecting Rad value
	int bandVel = 100;		//Band index in spectrum, affecting Vel value
	
	ofVec2f t_noise[NUM_CLOUD_POINTS];
	ofPoint p_cloud[NUM_CLOUD_POINTS];
	
	float t_Last = 0;
	
	ofImage img;
	
	ofTrueTypeFont font;
	
	bool b_HoriLine = false;
	
	/********************
	********************/
	ofVideoGrabber WebCam;
	bool b_UseWebCam = false;
	
	const int Cam_id;
	const int CamWidth;
	const int CamHeight;
	const int CamFps;
	
	ofFbo fbo_webCam;
	ofShader shader_Gray;
	
	/********************
	********************/
	ofFbo fbo_WorkSpace;
	ofFbo fbo_Send;
	
	/****************************************
	****************************************/
	void Refresh_vboVerts__Particle(const FFT& fft);
	void Refresh_vboColor__Particle();
	void Refresh_vboVerts__ArtSin(const FFT& fft);
	void Refresh_vboColor__ArtSin();
	void Refresh_vboVerts__FFT(const FFT& fft);
	void Refresh_vboColor__FFT(const FFT& fft);
	void setup_WebCam();
	void ClearFbo(ofFbo& fbo);
	
	void draw_img(ofFbo& fbo);
	void draw_WebCam(ofFbo& fbo);
	void draw_FFT(const FFT& fft, ofFbo& fbo);
	void draw_ArtSin(const FFT& fft, ofFbo& fbo);
	void draw_Particle(ofFbo& fbo);
	void draw_DancingCloud(ofFbo& fbo);
	
public:
	VISUALIZE(int _Cam_id, int _CamWidth, int _CamHeight, int _CamFps);
	~VISUALIZE();
	void setup(const FFT& fft);
	void update(const FFT& fft);
	void draw(const FFT& fft);
	void toggle_HoriLine();
	
	void update_spectrum(int id, double val);
	void update_phase(int id, double val);
	void Reset_EasyCam();
	
	ofFbo& get_fbo();
};

