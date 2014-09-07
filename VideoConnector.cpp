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

int VideoConnector::captureMultipleVideo(){
	   capture1=cvCaptureFromCAM(0);
	   cvSetCaptureProperty( capture1, CV_CAP_PROP_FRAME_WIDTH, 320 );
	   cvSetCaptureProperty( capture1, CV_CAP_PROP_FRAME_HEIGHT, 240 );
	   capture2=cvCaptureFromCAM(1);
	   cvSetCaptureProperty( capture2, CV_CAP_PROP_FRAME_WIDTH, 320 );
	   cvSetCaptureProperty( capture2, CV_CAP_PROP_FRAME_HEIGHT, 240);
	   if(!capture1)
	   {
	      printf("Cannot find device1!\n");
	      exit(0);
	   }
	   if(!capture2)
	   {
	      printf("Cannot find device2!\n");
	      exit(0);
	   }
	   IplImage * IplImage1=0;
	   IplImage * IplImage2=0;
	   cvNamedWindow("CAM1");
	   cvNamedWindow("CAM2");
	   while(1)
	   {
		  IplImage1=cvQueryFrame(capture1);
		  IplImage2=cvQueryFrame(capture2);
	      if(!IplImage1||!IplImage2)
	         break;
	      cvShowImage("CAM1",IplImage1);
	      cvShowImage("CAM2",IplImage2);
	      int key=cvWaitKey(10);
	      if(27==key)
	         break;
	   }
	   cvReleaseCapture(&capture1);
	   cvReleaseCapture(&capture2);
	   cvDestroyWindow("CAM1");
	   cvDestroyWindow("CAM2");

	   return 0;
}

int VideoConnector::captureVideo(int i){
	 this->capture1 = cvCaptureFromCAM( i );
	   cvSetCaptureProperty( capture1, CV_CAP_PROP_FRAME_WIDTH, 640 );
	   cvSetCaptureProperty( capture1, CV_CAP_PROP_FRAME_HEIGHT, 480 );
	    if(!this->capture1) cout << "No camera detected" << endl;

	    cvNamedWindow( "result", 1 );

	    if( capture1 ){
	        cout << "In capture ..." << endl;
	        for(;;){
	            IplImage* iplImg = cvQueryFrame( capture1 );
	            frame1 = iplImg;
	            if( frame1.empty() ){
	            	 cout << "No image captured" << endl;
	            	 break;
	            }
	            if( iplImg->origin == IPL_ORIGIN_TL )
	                frame1.copyTo( frameCopy1 );
	            else
	                flip( frame1, frameCopy1, 0 );
	            cvShowImage( "result", iplImg );
	            if( waitKey( 10 ) >= 0 )
	                cvReleaseCapture( &capture1 );
	        }
	    }
	    waitKey(0);
	    cvDestroyWindow("result");
	    return 0;
};
