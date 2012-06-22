#pragma once

//#include "ofMain.h"

#include "ofxSKT.h"
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "glWarper.h"
class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		ofxSKT skt;
		
	Mat imgRGB, imgMap, maskMap;
	VideoCapture capture;
	
	glWarper warper;
	
	bool bWarpEnable;
	
	bool cornersProc;
	
};

