/************************************************************
************************************************************/
#include "Projector.h"

/************************************************************
************************************************************/

/******************************
******************************/
PROJECTOR::PROJECTOR(){
	printf("PROJECTOR::ctr\n");
	fflush(stdout);
}

/******************************
******************************/
PROJECTOR::~PROJECTOR(){
	printf("PROJECTOR::dtr\n");
	fflush(stdout);
}

/******************************
******************************/
void PROJECTOR::exit(){
	printf("> Projector::exit\n");
	fflush(stdout);	
}

/******************************
******************************/
void PROJECTOR::setup(){
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("PROJECTOR");
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetWindowShape(WINDOW_W, WINDOW_H);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	ofSetCircleResolution(6);
}

/******************************
******************************/
void PROJECTOR::update(){

}

/******************************
******************************/
void PROJECTOR::draw(){
	ofBackground(0);
	ofSetColor(255);
	
	main_obj->Visualize.get_fbo().draw(0, 0, ofGetWidth(), ofGetHeight());
}

/******************************
******************************/
void PROJECTOR::keyPressed(int key){
}

/******************************
******************************/
void PROJECTOR::keyReleased(int key){

}

/******************************
******************************/
void PROJECTOR::mouseMoved(int x, int y ){

}

/******************************
******************************/
void PROJECTOR::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void PROJECTOR::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void PROJECTOR::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void PROJECTOR::mouseEntered(int x, int y){

}

/******************************
******************************/
void PROJECTOR::mouseExited(int x, int y){

}

/******************************
******************************/
void PROJECTOR::windowResized(int w, int h){

}

/******************************
******************************/
void PROJECTOR::gotMessage(ofMessage msg){

}

/******************************
******************************/
void PROJECTOR::dragEvent(ofDragInfo dragInfo){ 

}


