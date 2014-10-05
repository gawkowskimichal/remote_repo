/*
 * AlvarObjectTracker.h
 *
 *  Created on: 5 Oct 2014
 *      Author: mgw
 */


#ifndef ALVAROBJECTTRACKER_H_
#define ALVAROBJECTTRACKER_H_
#include "ObjectTracker.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

class AlvarObjectTracker : public ObjectTracker {
public:
	AlvarObjectTracker();
	virtual ~AlvarObjectTracker();
	void trackInPicture(Mat *picture);
	void trackInPictures(vector<Mat*> pictures);
	void trackInVideo(String filename);
};

#endif /* ALVAROBJECTTRACKER_H_ */
