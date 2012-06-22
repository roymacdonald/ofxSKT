
#include "ofxSKT.h"
#include "ofMain.h"
#include "tinyxml.h"
#include "tinystr.h"
using namespace cv;
using namespace TUIO;
//using namespace std;
ofxSKT::ofxSKT(){}
/////////////////////////////////////////////////////////////////////
ofxSKT::~ofxSKT(){
	delete letra;
	delete server;
	delete uno;
	/*delete iMap;
	delete nue;
	delete Base;
    delete paBlobs;
    delete  mask;
    delete  maskBack;
    delete  maskZona;
    delete  iRGB; 
    delete  iRGBaux;
	delete iRGBaux2;
    delete  sal;
    delete  sal0;
    delete  ACC;
	delete sqACC;
    delete ACCsq;
    delete varianza2;
    delete maskVar;
    delete maskACC;
	*/
	//_CrtDumpMemoryLeaks();
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::setup(){
	setup("localhost", 3333);
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::setup(const char * ip, int port){
	/*
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
	*/
	
g_min=0;         //Min threeshold for selection of pixels near interaction zone. Divided by 10000.
g_max=90;         //Max threeshold for selection of pixels near interaction zone. Divided by 10000.
g_maxBlob=100;       //Max blob area for filter. NOT IMPLEMENTED
g_minBlob=20;      //Min blob area for blob filter.
g_thre=50;           //Threeshold for last image. Almost not used...
g_mult=1;           //Multiplier for scale adjust of images. Not used...
g_var=300;         //Minimum variance for pixels in the background allowed. Divided by 100000 after.
g_save=true;
g_calBack=0;        //Background calibration toggle.
g_calZona=0;        //Zone calibration toggle.
g_showRGB=1;
g_showPaBlobs=0;
g_showFondo=0;
g_showMask=0;
g_showHelp=0;
g_showDepth=1;	
	
g_xx=95;           //Simple disparity between RGB and IR cameras
g_yy=98;
g_xxx=203;
g_yyy=199;
g_xy=100;
g_yx=104;
g_XY=0;
g_XF=0;
g_YF=0;
g_minVal=0;
g_maxVal=10000;
	
help="Press Ctrl+p for settings.\n"
	"Min Dist -> Min distance from interaction zone.\n"
	"Max Dist -> Max distance form interaction zone.\n"
	"Min Blob -> Min blob area.\n"
	"Deviation-> Min standard deviation background pixs.\n"
	"X Disp, Y Disp, etc... -> Affine Warp parameters.\n\n"
	"Key Shortcuts:\n"
	"M : show Mask.\n"
	"F : show Background.\n"
	"B : show Blobs.\n"
	"R : show RGB image.\n"
	"C : calibrate Background.\n"
	"Z : calibrate Interaction Zone.\n"
	"H : show this help;"
	"W : X<-->Y.\n"
	"X : Flip X.\n"
	"Y : Flip Y.\n";
	
	g_alpha=100;

	rgbx=0;
	rgby=0;
	rgbw=640;
	rgbh=480;
	
//Calibration Zone variables.
for(int i=0;  i<4; i++){
	esq[i]=cvPoint(0,0);
}
calibrado=false;
calibrando=false;
remapear=false;
		
	
	
	server = new TUIO::TuioServer(ip, port); //TUIO SERVER
  
    for(int h=0; h<4; h++)
    {
        esqc[h]=false;
    }
	
	g_storage = NULL;
	g_storage=cvCreateMemStorage(0); 
	blobs=0; 
	
    /*CBlobResult blobs;                                     //BLOB RESULTS
	 CBlob *currentBlob;                                    //BLOB, for passing through blob results
	 CBlobGetXCenter XOp;                                   //BLOB OPERATOR TO GET CENTER
	 CBlobGetYCenter YOp;*/
	/*

    /*Parameters for processing*/
	gui = new MyGUI();
	MyPanel* settingsPanel = new MyPanel("Settings", 0, 740);
	gui->addControl( settingsPanel );
	
	MySlider* minDistSlider = new MySlider("Min Dist", 10, 30, 300, 20, &g_min, 0, 100);
	settingsPanel->addControl( minDistSlider );

	MySlider* maxDistSlider = new MySlider("Max Dist", 330, 30, 300, 20, &g_max, 0, 10000);
	settingsPanel->addControl( maxDistSlider );
	
	MySlider* minBlobSlider = new MySlider("Min Blob", 650, 30, 300, 20, &g_minBlob, 0, 10000);
	settingsPanel->addControl( minBlobSlider );
	
	MySlider* devSlider = new MySlider("Deviation", 970, 30, 300, 20, &g_var, 0, 100000);
	settingsPanel->addControl( devSlider );
	
	MySlider* xxSlider = new MySlider("Sal Min Val", 10, 60, 300, 20, &g_minVal, 0, 10000);
	settingsPanel->addControl( xxSlider );

	MySlider* yySlider = new MySlider("Sal Max Val", 330, 60, 300, 20, &g_maxVal, 0, 10000);
	settingsPanel->addControl( yySlider );
	
	MySlider* xySlider = new MySlider("XY Disp", 650, 60, 300, 20, &g_xy, 0, 2000);
	settingsPanel->addControl( xySlider );
	
	MySlider* yxSlider = new MySlider("YX Disp", 970, 60, 300, 20, &g_yx, 0, 2000);
	settingsPanel->addControl( yxSlider );
	
	MySlider* xxxSlider = new MySlider("XXX Disp", 10, 90, 300, 20, &g_xxx, 0, 2000);
	settingsPanel->addControl( xxxSlider );
	
	MySlider* yyySlider = new MySlider("YYY Disp", 330, 90, 300, 20, &g_yyy, 0, 2000);
	settingsPanel->addControl( yyySlider );
	
	MySlider* alphaSlider = new MySlider("Alpha", 650, 90, 300, 20, &g_alpha, 0, 100);
	settingsPanel->addControl(alphaSlider );

	MySlider* threSlider = new MySlider("Threshold", 970, 90, 300, 20, &g_thre, 0, 255);
	settingsPanel->addControl( threSlider );
	
	MySlider* multSlider = new MySlider("sal mult", 10, 120, 300, 20, &g_mult, 0, 1.0f);
	settingsPanel->addControl( multSlider );

	
    /*Several image definitions needed...*/
	nue.allocate(640,480);
	Base.allocate(640,480);
    paBlobs.allocate(640,480);
	BlobsACC.allocate(640,480);
    mask.allocate(640,480);
    maskBack.allocate(640,480);
    maskZona.allocate(640,480);
    iRGB.allocate(640,480);
    iRGBaux.allocate(640,480);
    iRGBaux2.allocate(640,480);
	sal.allocate(640,480);
    sal0.allocate(640,480);
    ACC.allocate(640,480);
    sqACC.allocate(640,480);
    ACCsq.allocate(640,480);
    varianza2.allocate(640,480);
    maskVar.allocate(640,480);
    maskACC.allocate(640,480);
	iMap.allocate(640,480);
	maskIndex.allocate(640, 480);
	diffImg.allocate(640, 480);
	uno =  cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
    /////////////////////////////////////////////////////////////////
	
	nback=0;
	letra=new CvFont;
    cvInitFont(letra,CV_FONT_HERSHEY_SIMPLEX,0.5,0.5,0.0,2);
    warpAffin=cvCreateMat(2,3,CV_32FC1);
    timing=0;
    inicio=ofGetElapsedTimeMillis();
	n=0;
	
	enableMouseAndKey();
	
	imgUpdated=false;
	depthUpdated=false;
	
	bDoContours=false;
	
	listo=false;
	bAndMaskBlobs=true;
	bShowGUI=true;
	bShowAll=false;
	bWrapAffine=false;
	
	cvSetZero(BlobsACC.getCvImage());
	BlobsACC.flagImageChanged();
	
for (int i =0; i<5; i++) {
	for (int j=0; j<4; j++) {
		bShowMM[i][j]=false;
	}
}
	

}
/////////////////////////////////////////////////////////////////////
void ofxSKT::drawRGB(int x, int y){
	iRGB.draw(x, y);
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::drawDepth(int x, int y){
	mask.draw(x,y);
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::drawBlobs(int x, int y, int w, int h){
	paBlobs.draw(x,y,w,h);
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::drawBlobsACC(int x, int y, int w, int h){
	BlobsACC.draw(x,y,w,h);
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::updateDepthImg(Mat dpth, Mat validDpth){
	*iMap.getCvImage()=dpth;
	iMap.flagImageChanged();
	*mask.getCvImage()=validDpth;
	mask.flagImageChanged();
	depthUpdated=true;
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::updateColorImg(Mat colr){
	*iRGB.getCvImage()=colr;
	iRGB.flagImageChanged();
	imgUpdated=true;
}

/////////////////////////////////////////////////////////////////////
void ofxSKT::hacer0bordes(IplImage* in){
	/*hacer0bordes : Function to set the pixels in the border of the image to 0*/
	
    int x=0,y=0,wi=in->width,he=in->height;
    uchar* ptr = ((uchar*)(in->imageData + in->widthStep*(y+he-1)));
    for(int col=x; col<(x+wi); col++)
    {
        ptr[col]=0;
    }
    ptr=NULL;
    ptr = ((uchar*)(in->imageData+in->widthStep*(y)));
    for(int col=x; col<(x+wi); col++)
    {
        ptr[col]=0;
    }
    for(int row=y; row<(y+he); row++)
    {
        ptr=NULL;
        ptr=((uchar*)(in->imageData + in->widthStep*row));
        ptr[x]=0;
        ptr[x+wi-1]=0;
    }
	ptr=NULL;
}
/////////////////////////////////////////////////////////////////////
void ofxSKT::update(){
	grabImages();
	calibBackground();
	calibZone();
	if (listo) {
		if (bShowAll) {
			if (bShowMM[0][0]) {
				ACC.setMinMax();
				ACC.flagImageChanged();
			}
			if (bShowMM[2][1]) {
				maskIndex.flagImageChanged();
				maskIndex.setMinMax();
			}
			if (bShowMM[2][2]) {
				varianza2.setMinMax();
				varianza2.flagImageChanged();
			}
			if (bShowMM[2][0]) {
				Base.setMinMax();
				Base.flagImageChanged();
			}
			
		}
	calcDiff();
	detectBlobs();
	updateTUIO();
	}
}
////////////////////////////////////////
void ofxSKT::grabImages(){
	//Image grabbing and first conversions.
/*	if( !capture.grab() )
	{
		cout << "Can not grab images." << endl;
		return;
	}
	else
	{
		capture.retrieve( imgMap, CV_CAP_OPENNI_DEPTH_MAP);
		capture.retrieve( maskMap, CV_CAP_OPENNI_VALID_DEPTH_MASK);
		*iMap.getCvImage()=imgMap;
		iMap.flagImageChanged();
		*mask.getCvImage()=maskMap;
		mask.flagImageChanged();*/
	
	if (imgUpdated && depthUpdated) {
		imgUpdated=false;
		depthUpdated=false;
		cvConvertScale(iMap.getCvImage(),nue.getCvImage(),(double)1/6000);
		//cvCopy(iMap.getCvImage(), nue.getCvImage(), mask.getCvImage());
		nue.flagImageChanged();
            if(bWrapAffine)
            {
                float* ptr = (float*)(warpAffin->data.ptr);
                ptr[0]=(float)g_xxx/100-1.0;
                ptr[1]=(float)g_xy/100-1.0;
                ptr[2]=(g_xx-100);
                ptr=NULL;
                ptr=(float*)(warpAffin->data.ptr + warpAffin->step);
                ptr[0]=(float)g_yx/100-1.0;
                ptr[1]=(float)g_yyy/100-1.0;
                ptr[2]=(g_yy-100);
                ptr=NULL;
              //  capture.retrieve(imgRGB,CV_CAP_OPENNI_BGR_IMAGE);
                //*iRGB.getCvImage()=imgRGB;
				//iRGB.flagImageChanged();
				cvWarpAffine(iRGB.getCvImage(),iRGBaux2.getCvImage(),warpAffin);
                cvConvertImage(nue.getCvImage(),iRGBaux.getCvImage(),CV_GRAY2RGB);
                cvAddWeighted(iRGBaux2.getCvImage(),(double)g_alpha/100,iRGBaux.getCvImage(),(1.0-(double)g_alpha/100),0.0,iRGB.getCvImage());
				iRGBaux2.flagImageChanged();
				iRGBaux.flagImageChanged();
				iRGB.flagImageChanged();

				if(remapear)
                {
                    int *num=new int[1];
                    num[0]=4;
                    CvPoint **e=new CvPoint*[1];
                    e[0]=esq;
                    cvPolyLine(iRGB.getCvImage(),e,num,1,1,cvScalar(255),2);
					delete e;
					delete num;
                }
            }
	}
}
/////////////////////////////////////////////
void ofxSKT::calibBackground(){
	/*Background setting with sampling over 100 frames. Background mask
	 *restricts pixels with less deviation than parameter set in "Parametros"
	 *window divided by 10000. It also accumulates valid depth valid pixels.*/
            if(g_save)
            {
                if(nback==0)
                {
                    cvSet(maskACC.getCvImage(),cvScalar(255));
                    cvSet(ACC.getCvImage(),cvScalar(0.0));
                    cvSet(maskVar.getCvImage(),cvScalar(255));
                    cvSet(sqACC.getCvImage(),cvScalar(0.0));
                    cvSet(ACCsq.getCvImage(),cvScalar(0.0));
					cvSet(uno, cvScalar(1));
					cvSet(maskIndex.getCvImage(), cvScalar(0.0));
					cvSet(varianza2.getCvImage(), cvScalar(0.0));
					cvSet(Base.getCvImage(), cvScalar(0.0));


                }
                if(nback>99)
                {
                    //cvConvertScale(ACC.getCvImage(),ACC.getCvImage(),(double)1/100);
                    //cvConvertScale(sqACC.getCvImage(),sqACC.getCvImage(),(double)1/100);
					
					cvDiv(ACC.getCvImage(), maskIndex.getCvImage(), ACC.getCvImage());
					cvDiv(sqACC.getCvImage(), maskIndex.getCvImage(), sqACC.getCvImage());
					
                    cvMul(ACC.getCvImage(),ACC.getCvImage(),ACCsq.getCvImage());
                    cvAbsDiff(sqACC.getCvImage(),ACCsq.getCvImage(),varianza2.getCvImage());
					cvPow(varianza2.getCvImage(),varianza2.getCvImage(),0.5);
                    cvDiv(varianza2.getCvImage(),ACC.getCvImage(),varianza2.getCvImage());
                    cvThreshold(varianza2.getCvImage(),maskVar.getCvImage(),(double)g_var,255,CV_THRESH_BINARY_INV);
                    cvAnd(maskACC.getCvImage(),maskVar.getCvImage(),maskACC.getCvImage());
                    cvCopy(ACC.getCvImage(),Base.getCvImage());
					//printMinMax(Base.getCvImage(), "Base pre mask");
										
					cvCopy(maskACC.getCvImage(),maskBack.getCvImage());
                    g_save=false;
                    nback=-1;
				
			
					varianza2.flagImageChanged();
					maskACC.flagImageChanged();
					sqACC.flagImageChanged();
					ACCsq.flagImageChanged();
					ACC.flagImageChanged();
					maskIndex.flagImageChanged();
					maskVar.flagImageChanged();
						
				
						
                }
                else
                {
                    cvAnd(mask.getCvImage(),maskACC.getCvImage(),maskACC.getCvImage());
					//cvOr(mask.getCvImage(),maskACC.getCvImage(),maskACC.getCvImage());

//                  cvAcc(nue.getCvImage(),ACC.getCvImage(),maskACC.getCvImage());
					cvAcc(nue.getCvImage(),ACC.getCvImage(),mask.getCvImage());
					cvAcc(uno, maskIndex.getCvImage(), mask.getCvImage());
//                  cvSquareAcc(nue.getCvImage(),sqACC.getCvImage(),maskACC.getCvImage());
					cvSquareAcc(nue.getCvImage(),sqACC.getCvImage(),mask.getCvImage());

                    n=sprintf(text,"Calibrando Background... %d",nback);
                    cvPutText(nue.getCvImage(),text,cvPoint(40,100),letra,cvScalar(0,0,255));
                }
                nback++;
            }
}
///////////////////////////////////////////////////////////
void ofxSKT::calibZone(){		
	if(!g_save){
				
            /*Calibration for valid zone. This reduces noise by selecting the interaction zone.
             *It also calibrates valid range for the output coordinates of the blobs.*/
            if(calibrando)
            {
                
                n=sprintf(text,"Calibrate Zone: (0->UpL) / (1->UpR) / (2->DownR) / (3->DownL)");
                cvPutText(nue.getCvImage(),text,cvPoint(40,100),letra,cvScalar(0,0,255));
                int zonaActual=0;
                for(int r=0; r<4; r++)
                {
                    if(esqc[r])
                    {
                        cvCircle(nue.getCvImage(),esq[r],20,cvScalar(0,0,(int)(60*(r+1))),5);
                    }
                    else
                    {
                        zonaActual=r;
                        r=4;
                    }
                }
                n=sprintf(text2,"Click in Zone : %d",zonaActual);
                cvPutText(nue.getCvImage(),text2,cvPoint(40,80),letra,cvScalar(0,255,255));
            }
            if(calibrado)
            {
                cvSetZero(maskZona.getCvImage());
                cvFillConvexPoly(maskZona.getCvImage(),esq,4,cvScalar(255));
                cvAnd(maskZona.getCvImage(),maskBack.getCvImage(),maskBack.getCvImage());
                a[0]=(double)(esq[1].y-esq[0].y)/(esq[1].x-esq[0].x);
                b[0]=esq[1].y-a[0]*esq[1].x;
                a[1]=(double)(esq[3].y-esq[2].y)/(esq[3].x-esq[2].x);
                b[1]=esq[2].y-a[1]*esq[2].x;
                a[2]=(double)(esq[2].x-esq[1].x)/(esq[2].y-esq[1].y);
                b[2]=esq[2].x-a[2]*esq[2].y;
                a[3]=(double)(esq[3].x-esq[0].x)/(esq[3].y-esq[0].y);
                b[3]=esq[0].x-a[3]*esq[0].y;
                remapear=true;
                calibrado=false;
			//	*setdone=true;
				listo=true;
				g_showRGB=0;
            }
			}
	}
///////////////////////////////////////////////////////////////////////
void ofxSKT::calcDiff(){
            /*This part calculates the difference between background and new depth image
             *and normalizes the difference according to depth. Then a min and max threeshold
             *is used to select pixels near the interaction zone. */
            
			//cvAnd(mask.getCvImage(),maskBack.getCvImage(),mask.getCvImage());
            
			cvSetZero(sal0.getCvImage());
			cvSetZero(diffImg.getCvImage());
			
	
			cvAnd(mask.getCvImage(),maskBack.getCvImage(),mask.getCvImage());
			
			
			cvSub(Base.getCvImage(), nue.getCvImage(),diffImg.getCvImage(),mask.getCvImage());
            
			cvDiv(diffImg.getCvImage(),Base.getCvImage(),sal.getCvImage()); //Very simple kinect touch distance invariance
            
			cvThreshold(sal.getCvImage(),sal.getCvImage(),(double)g_min/10000,255.0,CV_THRESH_TOZERO);
            cvThreshold(sal.getCvImage(),sal.getCvImage(),(double)g_max/10000,255.0,CV_THRESH_TOZERO_INV);
			//cvThreshold(sal.getCvImage(),paBlobs.getCvImage(),(double)g_max/10000,255.0,CV_THRESH_BINARY);

			
			//scale = 255.0f/(g_maxVal-g_minVal);
			//cout<<"paBlobs scale: "<< ofToString(scale)<<endl;
			
			cvConvertScale(sal.getCvImage(),paBlobs.getCvImage(),2550,0);
			
			//double min_val;
			//double max_val;
			
			//cvMinMaxLoc(paBlobs.getCvImage(), &min_val, &max_val);
			//cvMinMaxLoc(sal.getCvImage(), &min_val, &max_val);

	
	//cout <<	"paBlobs min: "<<ofToString(min_val)<<"  max: "<<ofToString(max_val)<<endl;
			//cvConvertScale(diffImg.getCvImage(),sal0.getCvImage(),255*g_mult);
	
			//nue.flagImageChanged();
			//cBase.flagImageChanged();
			if (bShowMM[4][0]==true) {
			diffImg.setMinMax();
			}
			if (bShowMM[2][2]==true) {
			sal.setMinMax();
			if (bShowMM[3][2]==true) {
			}
			sal0.setMinMax();
			}
			diffImg.flagImageChanged();
			sal.flagImageChanged();
			sal0.flagImageChanged();
			paBlobs.flagImageChanged();
			
}
///////////////////////////////////////////////////////////////////////
void ofxSKT::detectBlobs(){
            /*This part prepares the selected pixels for blob detection by selecting only the valid values and
             *then procedes to calculate the blobs. Then a min area filter is performed on the blobs.*/
	hacer0bordes(paBlobs.getCvImage());
            
	cvThreshold(paBlobs.getCvImage(),paBlobs.getCvImage(),g_thre,255,CV_THRESH_BINARY);
	cvOr(paBlobs.getCvImage(), BlobsACC.getCvImage(), BlobsACC.getCvImage());
	BlobsACC.flagImageChanged();
	
	if (bAndMaskBlobs) {
			//cvAnd(mask.getCvImage(),paBlobs.getCvImage(),paBlobs.getCvImage());	
	}
	paBlobs.flagImageChanged();
		
	if(g_showPaBlobs==1 && calibrando==0){
			// cvShowImage("To Blobs", paBlobs);
	}
	blobs=0; 
	cvClearMemStorage( g_storage );
	if(bDoContours){
	cvFindContours(paBlobs.getCvImage(), g_storage, &blobs ,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
	}
}
///////////////////////////////////////////////////////////////////////
void ofxSKT::updateTUIO(){
            /*This part makes the correspondance between old blobs and new ones (though distance) and
             *adds new ones, removes old ones not related to new ones and updates new blobs related
             *to old ones. Each part adds to the final TUIO message.*/
            
			list< TuioObject * > anteriores=server->getTuioObjects();
            list< TuioObject * >::iterator cursor;
            
            int maxID=0;
			int BlbNum=0;
			while(blobs){
				cvMoments (blobs, &moments,0);
				if(moments.m00>g_minBlob && BlbNum<MAX_BLOBS){
                nuevo[BlbNum]=true;
                //cX=currentBlob->MinX() + (( currentBlob->MaxX() - currentBlob->MinX() ) / 2.0);//XOp(currentBlob);
                //cY=currentBlob->MinY() + (( currentBlob->MaxY() - currentBlob->MinY() ) / 2.0);//YOp(currentBlob);
				cX=(moments.m10 / moments.m00);
				cY=(moments.m01 / moments.m00);
				float z=iMap.getPixelsAsFloats()[cX+(cY*iMap.width)];
                if(remapear)
                {
                    di[0]=fabs(cY-a[0]*cX-b[0]);
                    di[1]=fabs(cY-a[1]*cX-b[1]);
                    di[2]=fabs(cX-a[2]*cY-b[2]);
                    di[3]=fabs(cX-a[3]*cY-b[3]);
                    if(g_XY==1)
                    {
                        curX=di[0]/(di[0]+di[1]);
                        curY=di[3]/(di[2]+di[3]);
                    }
                    else
                    {
                        curY=di[0]/(di[0]+di[1]);
                        curX=di[3]/(di[2]+di[3]);
                    }
                    if(g_XF==1)
                    {
                        curX=1.0-curX;
                    }
                    if(g_YF==1)
                    {
                        curY=1.0-curY;
                    }
                }
                else
                {
                    if(g_XY==1)
                    {
                        curX=(double)cX/640;
                        curY=(double)cY/480;
                    }
                    else
                    {
                        curY=(double)cX/640;
                        curX=(double)cY/480;
                    }
                    if(g_XF==1)
                    {
                        curX=1.0-curX;
                    }
                    if(g_YF==1)
                    {
                        curY=1.0-curY;
                    }
                }
                nuevos[BlbNum][0]=curX;
                nuevos[BlbNum][1]=curY;
                nuevos[BlbNum][2]=cX;
                nuevos[BlbNum][3]=cY;
				nuevos[BlbNum][4]=z;
	
                for (cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
                {
                    distancias[BlbNum][(*cursor)->getSymbolID()]=sqrt(pow((curX-(*cursor)->getX()),2)+pow((curY-(*cursor)->getY()),2));
                    //cout << distancias[i][(*cursor)->getCursorID()] << endl;
                    //maxID=max(maxID,(*cursor)->getCursorID());
                }
                if(g_showRGB==1)
                {
                    cvCircle(iRGB.getCvImage(),cvPoint(cX,cY),sqrt((double)moments.m00/3.14),cvScalar(255,0,0),-1);
                }
				BlbNum++;
				}
				blobs=blobs->h_next;
            }
            for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
            {
                viejo[(*cursor)->getSymbolID()]=false;
            }
            bool buscar=true;

            while(buscar)
            {
                buscar=false;
                float minDist=10000;
                int indiceCercano=90000;
                int ID,IDD;
                for (int i = 0; i < BlbNum; i++ )
                {
                    for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
                    {
                        ID=(int)(*cursor)->getSymbolID();
                        if(minDist>distancias[i][ID] && nuevo[i] && !viejo[ID])
                        {
                            minDist=distancias[i][ID];
                            indiceCercano=i;
                            IDD=ID;
                            buscar=true;
                        }
                    }
                }
                if(buscar)
                {
                    nuevo[indiceCercano]=false;
                    viejo[IDD]=true;
                    vN[IDD]=indiceCercano;
                }
            }
            time=TuioTime::getSessionTime();
            server->initFrame(time);
            for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
            {
                int ID=(*cursor)->getSymbolID();
                if(viejo[ID])
                {
                   // cout << "UPDATE : " << ID << "(" << nuevos[vN[ID]][0] << "," << nuevos[vN[ID]][1] << ")"<< endl;
                    server->updateTuioObject(*cursor,nuevos[vN[ID]][0],nuevos[vN[ID]][1],nuevos[vN[ID]][4]);
                    if(g_showRGB==1)
                    {
                        
                        int nn=sprintf(ind,"%d",ID);
                        cvPutText(iRGB.getCvImage(),ind,cvPoint((int)nuevos[vN[ID]][2],(int)nuevos[vN[ID]][3]),letra,cvScalar(0,255,0));
                    }
                }
            }
            for (int i = 0; i < BlbNum; i++ )
            {
                if(nuevo[i])
                {
                    TuioObject* tcur=server->addTuioObject(BlbNum,(float)nuevos[i][0],(float)nuevos[i][1],(float)nuevos[i][4]);
                    if(g_showRGB==1)
                    {
                        n=sprintf(ind,"%d",(int)tcur->getSymbolID());
                        cvPutText(iRGB.getCvImage(),ind,cvPoint((int)nuevos[i][2],(int)nuevos[i][3]),letra,cvScalar(0,255,0));
                    }
                 //   cout << "ADD : " << tcur->getSymbolID() << endl;
                }
            }
            for(cursor = anteriores.begin(); cursor != anteriores.end( ); cursor++ )
            {
                int ID=(*cursor)->getSymbolID();
                if(!viejo[ID])
                {
                    server->removeTuioObject(*cursor);
                    cout << "REMOVE : " << ID << endl;
                }
            }
            server->commitFrame();
}
///////////////////////////////////////////////////////////////////////
void ofxSKT::draw(){		
	/*This is for calibration and debug. Selects which intermediate images to show and prints the
	 *calculated mean FPS in 100 frames*/
	if (bShowAll) {
		ofSetColor(255, 255, 255);
		      ACC.draw(   0, 0,   288, 216); ofDrawBitmapString("A C C",                10,10);	
		      nue.draw( 288, 0,   288, 216); ofDrawBitmapString("N U E",               298,10);
			 Base.draw( 576, 0,   288, 216); ofDrawBitmapString("B A S E",             586,10); 
		  paBlobs.draw( 864, 0,   288, 216); ofDrawBitmapString("p a B L O B S",       874,10);
	      diffImg.draw(1152, 0,   288, 216); ofDrawBitmapString("D I F F  I M G"     ,1164,10);
		
		     mask.draw(   0, 216, 288, 216); ofDrawBitmapString("M A S K",             10,226);
		 maskBack.draw( 288, 216, 288, 216); ofDrawBitmapString("M A S K  B A C K",   298,226);
		maskIndex.draw( 576, 216, 288, 216); ofDrawBitmapString("M A S K   I N D E X",586,226); 
		  maskVar.draw( 864, 216, 288, 216); ofDrawBitmapString("M A S K  V A R",     874,226);
		  maskACC.draw(1152, 216, 288, 216); ofDrawBitmapString("M A S K  A C C",    1164,226);
		
	  		 iRGB.draw(   0, 432, 288, 216); ofDrawBitmapString("i R G B",           10,422);
		varianza2.draw( 288, 432, 288, 216); ofDrawBitmapString("V A R 2",            298,422);
		      sal.draw( 576, 432, 288, 216); ofDrawBitmapString("S A L",              586,442);
			 sal0.draw( 864, 432, 288, 216); ofDrawBitmapString("S A L 0",            874,442); 
			sqACC.draw(1152, 432, 288, 216); ofDrawBitmapString("s q A C C",		 1164,442);
		
// 		     iMap.draw( 864, 648, 288, 216); ofDrawBitmapString("i M A P",			   10,658);		
//		 iRGBaux2.draw(   0, 648, 288, 216); ofDrawBitmapString("i R G B a u x 2",    298,658);
//		 maskZona.draw(   0, 648, 288, 216); ofDrawBitmapString("M A S K  Z O N E",   586,658);
//		     iRGB.draw( 576, 648, 288, 216); ofDrawBitmapString("i R G B",            874,658); 
//		  iRGBaux.draw( 864, 648, 288, 216); ofDrawBitmapString("i R G B a u x",     1164,658);*/
		
		// 288 216
		
		ofDrawBitmapString("paBlobs min: " + ofToString(min_val) + "  max: " + ofToString(max_val)+"paBlobs scale: " + ofToString(scale) + "\n",20, 700);
	}else{
	
			if (g_showDepth==1 && g_showPaBlobs==0) {
				mask.draw(640, 0);
				ofDrawBitmapString("M A S K  I M A G E",660,20);	
			}
            if(g_showRGB==1){
				nue.draw(rgbx, rgby,rgbw,rgbh);
				ofDrawBitmapString("D E P T H   I M A G E",rgbx+20,rgby+20);	
            }
            if(g_showPaBlobs==1 && calibrando==0){
                paBlobs.draw(320, 480, 320, 240);
				ofDrawBitmapString("B L O B S",350,500);
            }
            if(g_showFondo==1 && calibrando==0){
				Base.draw(0, 480, 320, 240);
				ofDrawBitmapString("B A C K G R O U N D",30,500);
            }
            if(g_showMask==1 && calibrando==0){
				maskBack.draw(640, 480, 320, 240);
				ofDrawBitmapString("B A C K G R O U N D   M A S K",670,500);
            }
			if (g_showHelp==1) {
				ofDrawBitmapString(help, 20,500);
			}
            if(timing==99)
            {
                fin=ofGetElapsedTimeMillis();
                cout << "FPS reales : " << (double)100000/(double)(fin-inicio) << endl;
                timing=0;
                inicio=ofGetElapsedTimeMillis();
            }
            else
            {
                timing++;
            }
	}
	if (bShowGUI) {
		gui->draw();
	}
	
	
	
            //////////////////////////////////////////////////////////////////////////////////////////////
	
      
    }
//--------------------------------------
void ofxSKT::toogleShowAll(){
	bShowAll^=true;
}
//--------------------------------------
//Background calibration starting point.
void ofxSKT::startBackgroundCalib(){
	g_save=true;
	nback=0;
}
//--------------------------------------
//Zone calibration starting point.
void ofxSKT::startZoneCalib(){
	calibrando=true;
	for(int r=0; r<4; r++)
	{
		esqc[r]=false;
	}
	calibrado=false;
}
//--------------------------------------
void ofxSKT::setDone(bool * setd){
	setdone =setd;
}
//--------------------------------------
void ofxSKT::enableMouseAndKey(){
	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
	cout << "ofxSKT mouse and keys enabled."<<endl;
}
//--------------------------------------
void ofxSKT::disableMouseAndKey(){
	ofUnregisterKeyEvents(this);
	ofUnregisterMouseEvents(this);
	cout << "ofxSKT mouse and keys disabled."<<endl;

}
//--------------------------------------
void ofxSKT::mouseMoved(ofMouseEventArgs &args){}
//--------------------------------------
void ofxSKT::mouseDragged(ofMouseEventArgs &args){}
//--------------------------------------
void ofxSKT::mousePressed(ofMouseEventArgs &args){}
//--------------------------------------
void ofxSKT::mouseReleased(ofMouseEventArgs &args){
	/*Mouse events, mostly for calibration purposes*/
	if(calibrando){
		if (args.x>rgbx && args.x<rgbw+rgbx) {
			if (args.y>rgby && args.y<rgbh+rgby) {
				for(int g=0; g<4; g++){
					if(!esqc[g]){
						esq[g]=cvPoint(args.x,args.y);
						esqc[g]=true;
						if(g==3){
							calibrando=false;
							calibrado=true;
						}
						g=4;
					}
				}
			}
		}
	}
	if(bShowAll){
		bShowMM[(int)floor(float(args.x)/288)][(int)floor(float(args.y)/216)]^=true;
	cout << "Show"<<ofToString((int)floor(float(args.x)/288))<<"  "<<ofToString((int)floor(float(args.y)/216))<<endl;	
		
	}
}
//--------------------------------------
void ofxSKT::keyPressed(ofKeyEventArgs &args){}
//--------------------------------------
void ofxSKT::keyReleased(ofKeyEventArgs &args){
	switch (args.key) {
		case ' ':
			cvSetZero(BlobsACC.getCvImage());
			BlobsACC.flagImageChanged();
			break;

		case 'l':
		case 'L':
			disableMouseAndKey();
			break;
		case 'A':
		case 'a':
			toogleShowAll();
			break;
		case 'G':
		case 'g':
			bShowGUI^=true;
			break;
		case 'n':
		case 'N':
			bDoContours^=true;
			break;
		case 'm':
		case 'M':
			g_showMask=abs(g_showMask-1);
			break;
		case 'f':
		case 'F':
			g_showFondo=abs(g_showFondo-1);
			break;
		case 'b':
		case 'B':
			g_showPaBlobs=abs(g_showPaBlobs-1);
			break;
		case 'R':
		case 'r':
			g_showRGB=abs(g_showRGB-1);
			break;
		case 'd':
		case 'D':
			g_showDepth=abs(g_showRGB-1);
			break;
		case 'c':
		case 'C':
			startBackgroundCalib();
			break;
		case 'z':
		case 'Z':
			startZoneCalib();
			break;
		case 'h':
		case 'H':
			g_showHelp=abs(g_showHelp-1);
			break;
		case 'W':
		case 'w':
			g_XY=abs(g_XY-1);
			break;
		case 'x':
		case 'X':
			g_XF=abs(g_XF-1);
			break;
		case 'y':
		case 'Y':
			g_YF=abs(g_YF-1);
			break;
		case 'v':
		case 'V':
			bAndMaskBlobs^=true;
			break;
	}
}

/*FUNCTION TO SAVE PARAMETERS*/
void ofxSKT::saveParameters( )
{
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    TiXmlElement * root = new TiXmlElement( "Parameters" );
    TiXmlElement * valores = new TiXmlElement( "Values" );
    doc.LinkEndChild( root );
    root->LinkEndChild( valores );
    TiXmlElement * blobDet= new TiXmlElement( "Blob-Detection" );
    TiXmlElement * affineVal= new TiXmlElement( "Affine-Calibration" );
    valores->LinkEndChild( blobDet );
    valores->LinkEndChild( affineVal );
    blobDet->SetAttribute("Min-Distance", g_min);
    blobDet->SetAttribute("Max-Distance", g_max);
    blobDet->SetAttribute("Min-Blob-Area", g_minBlob);
    blobDet->SetAttribute("Background-Accepted-Variance", g_var);
    affineVal->SetAttribute("X-X", g_xx);
    affineVal->SetAttribute("Y-Y", g_yy);
    affineVal->SetAttribute("X-X-X", g_xxx);
    affineVal->SetAttribute("Y-Y-Y", g_yyy);
    affineVal->SetAttribute("X-Y", g_xy);
    affineVal->SetAttribute("Y-X", g_yx);
    affineVal->SetAttribute("Alpha", g_alpha);
    bool saveOk=doc.SaveFile( "settings.xml" );
    if(saveOk)
    {
        cout << "Parameters saved!" << endl;
    }
    else
    {
        cout << "Parameters could not be saved." << endl;
    }
}
/*Function to load parameters*/
void ofxSKT::loadParameters()
{
    int read=0;
    TiXmlDocument doc( "settings.xml" );
    bool loadOkay=doc.LoadFile();
    if(loadOkay)
    {
        TiXmlElement * root=doc.FirstChildElement();
        TiXmlElement * valores=root->FirstChildElement();
        TiXmlElement * blobDet=valores->FirstChildElement("Blob-Detection");
        TiXmlElement * affineVal=valores->FirstChildElement("Affine-Calibration");
        read=blobDet->QueryIntAttribute("Min-Distance",&g_min);
        read=blobDet->QueryIntAttribute("Max-Distance",&g_max);
        read=blobDet->QueryIntAttribute("Min-Blob-Area",&g_minBlob);
        read=blobDet->QueryIntAttribute("Background-Accepted-Variance",&g_var);
        read=affineVal->QueryIntAttribute("X-X",&g_xx);
        read=affineVal->QueryIntAttribute("Y-Y",&g_yy);
        read=affineVal->QueryIntAttribute("X-X-X",&g_xxx);
        read=affineVal->QueryIntAttribute("Y-Y-Y",&g_yyy);
        read=affineVal->QueryIntAttribute("X-Y",&g_xy);
        read=affineVal->QueryIntAttribute("Y-X",&g_yx);
        read=affineVal->QueryIntAttribute("Alpha",&g_alpha);
        cout << "Parameter file: settings.xml - Loaded!" << endl;
    }
    else
    {
        cout << "Parameter file: settings.xml - not found. Using default values." << endl;
    }
}

	
