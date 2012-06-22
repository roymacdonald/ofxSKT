/*
 *  ofxSKT.h
 *  ofxSKT
 *
 *  Created by Roy Macdonald on 9/4/11.
 *  Copyright 2011 micasa. All rights reserved.
 *
 */
#ifndef _OFX_SKT
#define _OFX_SKT
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"

#include "ofxOpenCv.h"

#include "TuioServer.h"
#include "TuioCursor.h"
#include "TuioTime.h"
#include "TuioClient.h"

#include "ofMain.h"
#include "MyGUI.h"
#define MAX_BLOBS 20

using namespace cv;

struct blobPos {
	int x;
	int y;
	int z;
	float mx;
	float my;
	int ID;
	bool nuevo;
	};

class ofxSKT{
public:
	ofxSKT();
	~ofxSKT();
	void setup();
	void setup(const char * ip, int port );
	void runCalibration();
	void update();
	void hacer0bordes(IplImage* in);

	
	void updateDepthImg(Mat dpth, Mat validDpth);
	void updateColorImg(Mat colr);
	
	void mouseReleased(ofMouseEventArgs &args);
	void mouseMoved(ofMouseEventArgs &args);
	void mouseDragged(ofMouseEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	
	void updateTUIO();
	
	void drawRGB(int x, int y);
	void drawDepth(int x, int y);

	void drawBlobs(int x, int y , int w, int h);
	void drawBlobsACC(int x, int y , int w, int h);
	
	void calcDiff();
	void detectBlobs();
	void calibZone();
	void calibBackground();
	void grabImages(); 
	
	void updateBlobs();
	
	void draw();
	
	void keyPressed(ofKeyEventArgs &key);
	void keyReleased(ofKeyEventArgs &key);
	
	void startZoneCalib();
	
	void startBackgroundCalib();
	
	void enableMouseAndKey();
	void disableMouseAndKey();
	void setDone(bool * setd);
	
	void toogleShowAll();
	
	void saveParameters();
	void loadParameters();
	void setupGUI();
	void setupImages();
	void useTextures(bool use);
	ushort getDepthPixelAt(int x, int y);
	bool* setdone;
	MyGUI* gui;
	
	int g_min;         //Min threeshold for selection of pixels near interaction zone. Divided by 10000.
	int g_max;         //Max threeshold for selection of pixels near interaction zone. Divided by 10000.
	int g_maxBlob;     //Max blob area for filter. NOT IMPLEMENTED
	int g_minBlob;     //Min blob area for blob filter.
	int g_thre;        //Threeshold for last image. Almost not used...
	int g_var;         //Minimum variance for pixels in the background allowed. Divided by 100000 after.
	int g_xx;           //Simple disparity between RGB and IR cameras
	int g_yy;
	int g_xxx;
	int g_yyy;
	int g_xy;
	int g_yx;
	int g_XY;
	int g_XF;
	int g_YF;
	int g_minVal;
	int g_maxVal;
	float g_mult;        //Multiplier for scale adjust of images. Not used...
	
	bool bSave;
	bool bCalibrateBackground;     //Background calibration toggle.
	bool bCalibrateZone;     //Zone calibration toggle.
	bool bShowRGB;
	bool bShowPaBlobs;
	bool bShowFondo;
	bool bShowMask;
	bool bShowHelp;
	bool bShowDepth;
	bool bSendAllCursors;
	
	
	string help;
	int g_alpha;
	int inicio,fin;
	CvPoint esq[4];
	bool esqc[4];
	bool calibrado;
	bool calibrando;
	bool remapear;
	double a[4];
	double b[4];
	double c[4];
	double di[4];
	
	TUIO::TuioServer *server;
	
	vector<blobPos> oldBlobs;
	vector<blobPos> newBlobs;
	vector<blobPos> updatedBlobs;
	vector<blobPos> allBlobs;
	
	bool listo;
	
	int rgbx, rgby, rgbh, rgbw;
	
	float distancias[MAX_BLOBS][MAX_BLOBS];
	float nuevos[MAX_BLOBS][5];
    bool nuevo[MAX_BLOBS];
	bool viejo[MAX_BLOBS];
    int vN[MAX_BLOBS];
	CvMemStorage* g_storage;
	CvSeq* blobs; 
	CvMoments moments;
	TUIO::TuioTime time;
	
//	Mat imgMap,maskMap,imgRGB;
	Mat imgDepth;
    
	
    ofxCvGrayscaleImage paBlobs;
    
    ofxCvGrayscaleImage mask;
    ofxCvGrayscaleImage maskBack;
    ofxCvGrayscaleImage maskZona;
	ofxCvGrayscaleImage maskVar;
    ofxCvGrayscaleImage maskACC;
	ofxCvGrayscaleImage BlobsACC;
	ofxCvColorImage iRGB;
    ofxCvColorImage iRGBaux;
    ofxCvColorImage iRGBaux2;

	ofxCvFloatImage iMap;
    ofxCvFloatImage nue;
	ofxCvFloatImage Base;
    ofxCvFloatImage sal;
    ofxCvFloatImage sal0;
    ofxCvFloatImage ACC;
    ofxCvFloatImage sqACC;
    ofxCvFloatImage ACCsq;
    ofxCvFloatImage varianza2;
	ofxCvFloatImage maskIndex;
	ofxCvFloatImage diffImg;

	ofImage blobsImg;
	
	IplImage * uno;
	
	bool bShowMM [5][4];
	
	
	double min_val;
	double max_val;
	float scale;
	
    /////////////////////////////////////////////////////////////////

	
	//VideoCapture capture;
	
    double curX,curY;                       //Final values for each cursors position in 0-1 range.
    int cX,cY;
    int nback;
	CvFont *letra;
    CvMat* warpAffin;
    int timing;
	char text3[40];
	char text[100];
	char text2[60];
	char ind[4];
	int n;
	
	bool bShowAll;
	bool bDoContours;
	bool bShowGUI;
	bool bAndMaskBlobs;
	bool imgUpdated;
	bool depthUpdated;
	bool bWrapAffine;
};
#endif