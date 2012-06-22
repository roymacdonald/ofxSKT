#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	skt.setup();
	warper.setup();
	try{
		capture.open(CV_CAP_OPENNI);
	}
	catch(exception e){
		cout << endl << "No existen dispositivos disponibles." << endl;
        return;
	}
	if( !capture.isOpened() )
    {
        cout << "No se pudo abrir el dispositivo de captura." << endl;
        return ;
    }
    cout << "OpenNI capture opened." << endl;
	cout << "FPS Nominal : " << capture.get(CV_CAP_PROP_FPS) << endl;
	
	ofBackground(0, 0, 0);
	cornersProc=true;
	bWarpEnable=true;
}
//--------------------------------------------------------------
void testApp::update(){
	if( !capture.grab() )
	{
		cout << "Can not grab images." << endl;
		return;
	}
	else
	{
	capture.retrieve( imgMap, CV_CAP_OPENNI_DEPTH_MAP);
	capture.retrieve( maskMap, CV_CAP_OPENNI_VALID_DEPTH_MASK);
	capture.retrieve (imgRGB, CV_CAP_OPENNI_BGR_IMAGE);
	
	skt.updateDepthImg(imgMap, maskMap);
	skt.updateColorImg(imgRGB);
	}
	skt.update();
	if (skt.listo && cornersProc) {
		for (int i=0; i<4; i++) {
			warper.corners[i].x=skt.esq[i].x/640.0f;
			warper.corners[i].y=skt.esq[i].y/480.0f;
		}
		warper.processMatrices();
		
		cornersProc=false;
		ofSetWindowPosition(1500, 0);
		skt.disableMouseAndKey();
		ofToggleFullscreen();
		
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if (skt.listo && !cornersProc && bWarpEnable) {
		warper.draw();
		skt.drawBlobsACC(0,0,ofGetWidth(),ofGetHeight());
	}else {

	skt.draw();	
}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
switch (key) {
	case 'w':
		bWarpEnable^=true;
		break;

	case 'a':
		skt.toogleShowAll();
		break;
	case 'K':
	case 'k':
		skt.enableMouseAndKey();
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	default:
		break;
}


}
//--------------------------------------------------------------
void testApp::keyPressed(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}