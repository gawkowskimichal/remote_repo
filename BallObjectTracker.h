/*
 * BallObjectTracker.h
 *
 *  Created on: 12 Oct 2014
 *      Author: mgw
 */

#ifndef BALLOBJECTTRACKER_H_
#define BALLOBJECTTRACKER_H_
#include "ObjectTracker.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Configuration.h"
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;


namespace TracingFramework {

class BallObjectTracker : public ObjectTracker {
public:
	Configuration *config;
	String calibrationFile;
	double marker_size;
	BallObjectTracker(Configuration conf, String calibfn);
	virtual ~BallObjectTracker();
	void trackInPicture(Mat picture);
	void trackInPictures(vector<Mat> pictures);
	void trackInVideo(String filename);
};

} /* namespace TracingFramework */

#endif /* BALLOBJECTTRACKER_H_ */
