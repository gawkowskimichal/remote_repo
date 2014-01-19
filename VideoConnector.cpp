/*
 * VideoConnector.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "VideoConnector.h"
using namespace std;
using namespace cv;

VideoConnector::VideoConnector() {
	// TODO Auto-generated constructor stub

};

VideoConnector::~VideoConnector() {
	// TODO Auto-generated destructor stub
};


int VideoConnector::captureVideo(){
	 this->capture = cvCaptureFromCAM( 0 );
	    if(!this->capture) cout << "No camera detected" << endl;

	    cvNamedWindow( "result", 1 );

	    if( capture ){
	        cout << "In capture ..." << endl;
	        for(;;){
	            IplImage* iplImg = cvQueryFrame( capture );
	            frame = iplImg;
	            if( frame.empty() ){
	            	 cout << "No image captured" << endl;
	            }
	            if( iplImg->origin == IPL_ORIGIN_TL )
	                frame.copyTo( frameCopy );
	            else
	                flip( frame, frameCopy, 0 );
	            cvShowImage( "result", iplImg );
	            if( waitKey( 10 ) >= 0 )
	                cvReleaseCapture( &capture );
	        }
	    }
	    waitKey(0);
	    cvDestroyWindow("result");
	    return 0;
};
