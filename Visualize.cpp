/************************************************************
************************************************************/
#include "Visualize.h"

/************************************************************
************************************************************/

/******************************
******************************/
VISUALIZE::VISUALIZE(int _Cam_id, int _CamWidth, int _CamHeight, int _CamFps)
: Cam_id(_Cam_id)
, CamWidth(_CamWidth)
, CamHeight(_CamHeight)
, CamFps(_CamFps)
{
	font.load("font/RictyDiminished-Regular.ttf", 10, true, true, true);
}

/******************************
******************************/
VISUALIZE::~VISUALIZE()
{
}

/******************************
******************************/
void VISUALIZE::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : Visualize\n");
	fflush(stdout);
	
	/********************
	********************/
	for ( int j=0; j<NUM_CLOUD_POINTS; j++ ) {
		t_noise[j].x = ofRandom( 0, 1000 );
		t_noise[j].y = ofRandom( 0, 1000 );
	}
	
	/********************
	********************/
	img.load("img.png");
	
	/********************
	********************/
	LastCol_Particle = Gui_Global->col_particle;
	LastCol_ArtSin = Gui_Global->col_ArtSin;
	
	VboSet__Particle.setup(NUM_PARTICLES);
	Refresh_vboVerts__Particle(fft);
	Refresh_vboColor__Particle();
	VboSet__Particle.update_vertex_color();
	
	VboSet__ArtSin.setup(AUDIO_BUF_SIZE * 4);
	Refresh_vboVerts__ArtSin(fft);
	Refresh_vboColor__ArtSin();
	VboSet__ArtSin.update_vertex_color();
	
	VboSet__FFT.setup(AUDIO_BUF_SIZE/2 * 4);
	Refresh_vboVerts__FFT(fft);
	Refresh_vboColor__FFT(fft);
	VboSet__FFT.update_vertex_color();
	
	/********************
	********************/
	EasyCam.setDistance(100);
	
	/********************
	********************/
	setup_WebCam();
	fbo_webCam.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	ClearFbo(fbo_webCam);
	shader_Gray.load( "shader/Gray.vert", "shader/Gray.frag" );
	
	/********************
	********************/
	fbo_WorkSpace.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	ClearFbo(fbo_WorkSpace);
	
	fbo_Send.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	ClearFbo(fbo_Send);
}

/******************************
******************************/
void VISUALIZE::setup_WebCam()
{
	/********************
	********************/
	printf("> setup web camera\n");
	fflush(stdout);
	
	/*****s***************
	細かく情報出してもらう
	********************/
	ofSetLogLevel(OF_LOG_VERBOSE);
    WebCam.setVerbose(true);
	
	vector< ofVideoDevice > Devices = WebCam.listDevices();// 上 2行がないと、List表示されない.
	
	/********************
	********************/
	if(Cam_id == -1){
		b_UseWebCam = false;
	}else{
		if(Devices.size() <= Cam_id){
			b_UseWebCam = false; return;
		}else{
			b_UseWebCam = true;
			WebCam.setDeviceID(Cam_id);
			printf("\n> WebCam set device id = %d\n", Cam_id);
			printf("> DeviceID     = %d\n", Devices[Cam_id].id);
			printf("> CamName      = %s\n", Devices[Cam_id].deviceName.c_str());
			printf("> HardwareName = %s\n", Devices[Cam_id].hardwareName.c_str());
			printf("> serialID     = %s\n", Devices[Cam_id].serialID.c_str());
			
			printf( "\n> WebCam size asked  = (%d, %d)\n", int(CamWidth), int(CamHeight) );
			/*
				https://openframeworks.cc/documentation/video/ofVideoGrabber/#show_setDesiredFrameRate
					Set's the desired frame rate of the grabber. This should be called before initGrabber(), which will try to initialize at the desired frame rate. 
					Not all frame rates will be supported, but this at least gives you some abilitity to try grab at different rates.
			*/
			WebCam.setDesiredFrameRate(CamFps);
			// WebCam.initGrabber(CAM_WIDTH, CAM_HEIGHT);
			// WebCam.initGrabber(CamWidth, CamHeight, true); // bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
			WebCam.setup(CamWidth, CamHeight, true); // bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
			printf( "> WebCam size actual = (%d, %d)\n\n", int(WebCam.getWidth()), int(WebCam.getHeight()) );
			fflush(stdout);
		}
	}
}

/******************************
******************************/
void VISUALIZE::ClearFbo(ofFbo& fbo)
{
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
}

/******************************
1--2
|  |
0--3
******************************/
void VISUALIZE::Refresh_vboVerts__Particle(const FFT& fft)
{
	for(int _h = 0; _h < PARTICLE_HEIGHT; _h++){
		for(int _w = 0; _w < PARTICLE_WIDTH; _w++){
			float val = fft.get_history(_h, _w) * Gui_Global->wave_Amp;
			VboSet__Particle.set_vboVerts( _h * PARTICLE_WIDTH + _w, (_w - PARTICLE_WIDTH/2) * Gui_Global->space_of_dot_x, (_h - PARTICLE_HEIGHT/2) * Gui_Global->space_of_dot_y , val );
		}
	}
}

/******************************
******************************/
void VISUALIZE::Refresh_vboColor__Particle()
{
	VboSet__Particle.set_singleColor(Gui_Global->col_particle);
	
	return;
}

/******************************
1--2
|  |
0--3
******************************/
void VISUALIZE::Refresh_vboVerts__ArtSin(const FFT& fft)
{
	const ofVec2f ofs(128, 500);
	const int width_of_bar = 2;
	const int space_per_bar = 2;
	const int BarHeight = 100;
	
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		VboSet__ArtSin.set_vboVerts( i * 4 + 0, ofs.x + i * space_per_bar,					ofs.y );
		VboSet__ArtSin.set_vboVerts( i * 4 + 1, ofs.x + i * space_per_bar,					ofs.y - fft.get_artSin(i) * BarHeight );
		VboSet__ArtSin.set_vboVerts( i * 4 + 2, ofs.x + i * space_per_bar + width_of_bar,	ofs.y - fft.get_artSin(i) * BarHeight );
		VboSet__ArtSin.set_vboVerts( i * 4 + 3, ofs.x + i * space_per_bar + width_of_bar,	ofs.y );
	}
}

/******************************
******************************/
void VISUALIZE::Refresh_vboColor__ArtSin()
{
	VboSet__ArtSin.set_singleColor(Gui_Global->col_ArtSin);
	
	return;
}

/******************************
1--2
|  |
0--3
******************************/
void VISUALIZE::Refresh_vboVerts__FFT(const FFT& fft)
{
	const ofVec2f ofs(0, 700);
	const int width_of_bar = 3;
	const int space_per_bar = 5;
	const int BarHeight = 100;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		VboSet__FFT.set_vboVerts( i * 4 + 0, ofs.x + i * space_per_bar,					ofs.y );
		VboSet__FFT.set_vboVerts( i * 4 + 1, ofs.x + i * space_per_bar,					ofs.y - fft.get_Gain(i) * BarHeight );
		VboSet__FFT.set_vboVerts( i * 4 + 2, ofs.x + i * space_per_bar + width_of_bar,	ofs.y - fft.get_Gain(i) * BarHeight );
		VboSet__FFT.set_vboVerts( i * 4 + 3, ofs.x + i * space_per_bar + width_of_bar,	ofs.y );
	}
}

/******************************
******************************/
void VISUALIZE::Refresh_vboColor__FFT(const FFT& fft)
{
	ofColor col;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		if ( i == bandRad || i == bandVel )	col = ofColor( 0, 255, 0 , 150);
		else								col = ofColor( 0, 100, 255, 150 );
		
		VboSet__FFT.setColor_perShape(4, i, col); // setColor_perShape(int NumPerShape, int id, const ofColor& color);
	}
}

/******************************
******************************/
void VISUALIZE::toggle_HoriLine()
{
	b_HoriLine = !b_HoriLine;
}

/******************************
******************************/
void VISUALIZE::Reset_EasyCam()
{
	EasyCam.reset();
	EasyCam.setDistance(100);
	
	EasyCam.enableMouseInput();
}

/******************************
******************************/
void VISUALIZE::update(const FFT& fft)
{
	/********************
	********************/
	float now = ofGetElapsedTimef();
	float dt = now - t_Last;
	dt = ofClamp( dt, 0.0, 0.1 );	
	t_Last = now;
	
	bandRad = (int)Gui_Global->bandRad;
	bandVel = (int)Gui_Global->bandVel;

	Rad = ofMap( fft.get_Gain(bandRad), 1, 3, 400, 800, Gui_Global->b_Limit_Rad_min );
	Vel = ofMap( fft.get_Gain(bandVel), 0, 0.1, 0.05, 0.5 );
	
	for (int j = 0; j < NUM_CLOUD_POINTS; j++) {
		t_noise[j].x += Vel * dt;
		t_noise[j].y += Vel * dt;
		
		p_cloud[j].x = ofSignedNoise( t_noise[j].x ) * Rad;		
		p_cloud[j].y = ofSignedNoise( t_noise[j].y ) * Rad;	
	}
	
	/********************
	********************/
	{
		Refresh_vboVerts__Particle(fft);
		VboSet__Particle.update_vertex();
		
		ofColor col_gui = Gui_Global->col_particle;
		if(col_gui != LastCol_Particle){
			Refresh_vboColor__Particle();
			VboSet__Particle.update_color();
			
			LastCol_Particle = Gui_Global->col_particle;
		}
	}
	
	/********************
	********************/
	{
		Refresh_vboVerts__FFT(fft);
		VboSet__FFT.update_vertex();
		
		Refresh_vboColor__FFT(fft);
		VboSet__FFT.update_color();
	}
	
	/********************
	********************/
	{
		Refresh_vboVerts__ArtSin(fft);
		VboSet__ArtSin.update_vertex();
		
		ofColor col_gui = Gui_Global->col_ArtSin;
		if(col_gui != LastCol_ArtSin){
			Refresh_vboColor__ArtSin();
			VboSet__ArtSin.update_color();
			
			LastCol_ArtSin = Gui_Global->col_ArtSin;
		}
	}
	
	/********************
	********************/
	if(Gui_Global->b_DispWebCam && b_UseWebCam){
		WebCam.update();
		if(WebCam.isFrameNew()){
			shader_Gray.begin();
			fbo_webCam.begin();
				ofBackground(0);
				ofSetColor(255);
				WebCam.draw(0, 0, fbo_webCam.getWidth(), fbo_webCam.getHeight());
			fbo_webCam.end();
			shader_Gray.end();
		}
	}
}

/******************************
******************************/
ofFbo& VISUALIZE::get_fbo()
{
	if(Gui_Global->b_Separate_Disp_Send)	return fbo_Send;
	else									return fbo_WorkSpace;
}

/******************************
******************************/
void VISUALIZE::draw(const FFT& fft)
{
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	/********************
	********************/
	{
		ClearFbo(fbo_WorkSpace);
		
		if(Gui_Global->b_DispImg)					draw_img(fbo_WorkSpace);
		if(Gui_Global->b_DispWebCam && b_UseWebCam)	draw_WebCam(fbo_WorkSpace);
		if(Gui_Global->b_DispFFT)					draw_FFT(fft, fbo_WorkSpace);
		if(Gui_Global->b_DispArtSin)				draw_ArtSin(fft, fbo_WorkSpace);
		if(Gui_Global->b_DispParticle)				draw_Particle(fbo_WorkSpace);
		if(Gui_Global->b_DispDanceCloud)			draw_DancingCloud(fbo_WorkSpace);
	}
	
	if(Gui_Global->b_Separate_Disp_Send){
		ClearFbo(fbo_Send);
		
		if(Gui_Global->b_SendImg)					draw_img(fbo_Send);
		if(Gui_Global->b_SendWebCam && b_UseWebCam)	draw_WebCam(fbo_Send);
		if(Gui_Global->b_SendFFT)					draw_FFT(fft, fbo_Send);
		if(Gui_Global->b_SendArtSin)				draw_ArtSin(fft, fbo_Send);
		if(Gui_Global->b_SendParticle)				draw_Particle(fbo_Send);
		if(Gui_Global->b_SendDanceCloud)			draw_DancingCloud(fbo_Send);
	}
	
	/********************
	設定 戻す
	********************/
	ofDisableDepthTest(); // need this to draw gui properly.
	
	/********************
	********************/
	fbo_WorkSpace.draw(0, 0, ofGetWidth(), ofGetHeight());
}

/******************************
******************************/
void VISUALIZE::draw_img(ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		ofSetColor(Gui_Global->col_Img, Gui_Global->col_Img, Gui_Global->col_Img, 255);
		img.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
}

/******************************
******************************/
void VISUALIZE::draw_WebCam(ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		ofSetColor(Gui_Global->col_WebCam, Gui_Global->col_WebCam, Gui_Global->col_WebCam, 255);
		fbo_webCam.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
}

/******************************
******************************/
void VISUALIZE::draw_FFT(const FFT& fft, ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		
		//Draw background rect for spectrum
		const int line_per = 5;
		const int BarHeight = 100;
		const int width_of_bar = 3;
		const int space_per_bar = 5;
		
		// 座布団 : spectrum
		ofSetColor(255, 255, 255, 20);
		ofFill();
		ofDrawRectangle( 0, 700, fbo.getWidth(), -BarHeight );
		
		// 縦 line
		char buf[256];
		ofSetColor(255, 255, 255, 100);
		for(int i = 0; i * space_per_bar * line_per < fbo.getWidth(); i++){
			int _x = i * space_per_bar * line_per; // i 本目のlineのx座標
			ofDrawLine(_x, 700, _x, 700 - BarHeight );
			
			sprintf( buf, "%d", i * line_per);
			font.drawString(buf, _x, 700 - BarHeight);
		}
		
		// 横 line
		if(b_HoriLine){
			ofSetColor(255, 0, 0, 200);
			ofDrawLine(0,ofGetAppPtr()-> mouseY, fbo.getWidth(), ofGetAppPtr()->mouseY);	// ofApp でないので、mouseYは使えず、ofGetAppPtr()->mouseY とする必要がある。
		}
		
		ofSetColor(255);
		// glPointSize(Gui_Global->particleSize);
		VboSet__FFT.draw(GL_QUADS);
	fbo.end();
}

/******************************
******************************/
void VISUALIZE::draw_ArtSin(const FFT& fft, ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		
		ofSetColor(255);
		
		// glPointSize(Gui_Global->particleSize);
		VboSet__ArtSin.draw(GL_QUADS);
	fbo.end();
}

/******************************
******************************/
void VISUALIZE::draw_Particle(ofFbo& fbo)
{
	fbo.begin();
		if(Gui_Global->b_depthTest)	ofEnableDepthTest();
		else						ofDisableDepthTest();
		
		// ofSetHexColor(0xffffff);
		ofSetColor(255);
		
		if(Gui_Global->b_UseEasyCam){
			EasyCam.begin();
		}else{
			ofPushMatrix();
			ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
		}
		
		glPointSize(Gui_Global->particleSize);
		VboSet__Particle.draw(GL_POINTS);
		// VboSet__Particle.draw(GL_LINES);
		
		if(Gui_Global->b_UseEasyCam)	EasyCam.end();
		else							ofPopMatrix();
	fbo.end();
}

/******************************
******************************/
void VISUALIZE::draw_DancingCloud(ofFbo& fbo)
{
	fbo.begin();
		ofDisableDepthTest();
		
		ofPushMatrix();
			ofTranslate( fbo.getWidth() / 2, fbo.getHeight() / 2 );
		
			//Draw points
			ofSetColor(Gui_Global->col_DancingCloud);
			
			ofFill();
			for (int i=0; i<NUM_CLOUD_POINTS; i++) {
				ofDrawCircle( p_cloud[i], 2 );
			}
		
			//Draw lines between near points
			glLineWidth(1);
			float thresh_dist = 40;
			for (int j=0; j<NUM_CLOUD_POINTS; j++) {
				for (int k=j+1; k<NUM_CLOUD_POINTS; k++) {
					if ( ofDist( p_cloud[j].x, p_cloud[j].y, p_cloud[k].x, p_cloud[k].y ) < thresh_dist ) {
						ofDrawLine( p_cloud[j], p_cloud[k] );
					}
				}
			}
		
		ofPopMatrix();
	fbo.end();
}
