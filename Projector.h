/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "ofApp.h"

/************************************************************
************************************************************/
class PROJECTOR : public ofBaseApp{
private:
	enum{
		WINDOW_W = 1280,
		WINDOW_H = 720,
	};
	
public:
	PROJECTOR();
	~PROJECTOR();
	void exit();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	shared_ptr<ofApp> main_obj;
};


