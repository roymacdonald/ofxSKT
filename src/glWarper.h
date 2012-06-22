#ifndef _GL_WARPER
#define _GL_WARPER

#define OF_ADDON_USING_OFOPENCV

#include "ofMain.h"
//#include "ofAddons.h"


class glWarper{
	
public:
		
	void setup();
	void draw();
		
	
	void mouseDragged(ofMouseEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseReleased(ofMouseEventArgs &args);
	void mouseMoved(ofMouseEventArgs &args);
	void processMatrices();
	
	void activate();
	void deactivate();
	bool isActive();
	
	bool bSetInverse;
		ofPoint corners[4];
private:
	bool active;

	int whichCorner;
	GLfloat myMatrix[16];
};

#endif	

