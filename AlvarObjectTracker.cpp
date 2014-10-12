/*
 * AlvarObjectTracker.cpp
 *
 *  Created on: 5 Oct 2014
 *      Author: mgw
 */

#include "AlvarObjectTracker.h"
#include "Camera.h"

using namespace cv;
using namespace alvar;
using namespace TracingFramework;

AlvarObjectTracker::AlvarObjectTracker(Configuration conf, String calibfn) {
	config = &conf;
	marker_size = atof(config->getValueByKey("markerSize").c_str());
	double width = atof(config->getValueByKey("resolution_y").c_str());
	double height = atof(config->getValueByKey("resolution_x").c_str());
	cam.SetCalib(calibfn.c_str(), width, height);
	// TODO Auto-generated constructor stub

}

AlvarObjectTracker::~AlvarObjectTracker() {
	// TODO Auto-generated destructor stub
}

void AlvarObjectTracker::trackInPicture(Mat picture){
	IplImage image = picture;
	IplImage *image_tmp = &image;
	bool flip_image = (image_tmp->origin?true:false);
	if (flip_image) {
		cvFlip(image_tmp);
		image_tmp->origin = !image_tmp->origin;
	}
	static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
	marker_detector.SetMarkerSize(marker_size);
	marker_detector.Detect(image_tmp, &cam, true, true);

	return;
};


void AlvarObjectTracker::trackInPictures(vector<Mat> pictures){
	for(std::vector<Mat>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		trackInPicture(*it);
	}
};


void AlvarObjectTracker::trackInVideo(String filename){
	return;
};
