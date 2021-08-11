/************************************************************
************************************************************/
#include <ofMain.h>
#include "ofApp.h"
#include "Projector.h"
#include <ofAppGLFWWindow.h>

/************************************************************
************************************************************/
int main( int argc, char** argv ){
	/********************
	comment out for multi-window
	********************/
	// ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	/********************
	********************/
	int soundStream_Input_DeviceId = -1;
	int soundStream_Output_DeviceId = -1;
	
	int Cam_id = -1;
	int CamWidth	= 1920;
	int CamHeight	= 1080;
	int CamFps = 30;

	/********************
	********************/
	printf("---------------------------------\n");
	printf("> parameters\n");
	printf("\t-i      Audio in(-1)\n");
	printf("\t-o      Audio out(-1)\n");
	printf("\t-c      camera_id(-1)\n");
	printf("\t-w      cam_width(1920)\n");
	printf("\t-h      cam_height(1080)\n");
	printf("\t-CamFps CamFps(30)\n");
	printf("---------------------------------\n");
	
	for(int i = 1; i < argc; i++){
		if( strcmp(argv[i], "-i") == 0 ){
			if(i+1 < argc) { soundStream_Input_DeviceId = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-o") == 0 ){
			if(i+1 < argc) { soundStream_Output_DeviceId = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-c") == 0 ){
			if(i+1 < argc) { Cam_id = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-w") == 0 ){
			if(i+1 < argc) { CamWidth = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-h") == 0 ){
			if(i+1 < argc) { CamHeight = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-CamFps") == 0 ){
			if(i+1 < argc) { CamFps = atoi(argv[i+1]); i++;}
		}
	}
	
	/********************
	********************/
	printf("> parameters\n");
	printf("sound_In  = %d\n", soundStream_Input_DeviceId);
	printf("sound_Out = %d\n", soundStream_Output_DeviceId);
	printf("Cam_id    = %d\n", Cam_id);
	printf("CamWidth  = %d\n", CamWidth);
	printf("CamHeight = %d\n", CamHeight);
	printf("CamFps    = %d\n", CamFps);
	fflush(stdout);
	
	
	/********************
	comment out for multi-window
	********************/
	// ofRunApp(new ofApp( soundStream_Input_DeviceId, soundStream_Output_DeviceId, Cam_id, CamWidth, CamHeight, CamFps ));
	
	/********************
	C++11スマートポインタ入門
		https://qiita.com/hmito/items/db3b14917120b285112f
	********************/
	ofGLFWWindowSettings settings;

    settings.setSize(600, 600);
	settings.setPosition(glm::vec2(300,100));
	settings.resizable = true;
	// settings.shareContextWith = secondWindow;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	
    settings.setSize(300, 300);
	settings.setPosition(glm::vec2(0,0));
	settings.resizable = true;
	settings.shareContextWith = mainWindow; // texure(Graphic memory)領域は、mainWindowと共有する : この設定がないと、fboなど、texture関連memoryの やり取りができない。
	shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);
	
	shared_ptr<ofApp> mainApp( new ofApp(soundStream_Input_DeviceId, soundStream_Output_DeviceId, Cam_id, CamWidth, CamHeight, CamFps) );
	shared_ptr<PROJECTOR> ProjectorApp( new PROJECTOR );
	ProjectorApp->main_obj = mainApp;

	ofRunApp(mainWindow, mainApp);
	ofRunApp(secondWindow, ProjectorApp);
	
	ofRunMainLoop();
}



