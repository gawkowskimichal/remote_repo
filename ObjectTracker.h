/*
 * ObjectTracker.h
 *
 *  Created on: 5 Oct 2014
 *      Author: mgw
 */

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>

#ifndef OBJECTTRACKER_H_
#define OBJECTTRACKER_H_


using namespace cv;
using namespace std;
namespace TracingFramework {
class ObjectTracker {
public:
	ObjectTracker();
	virtual ~ObjectTracker();
	virtual void trackInPicture(Mat picture) = 0;
	virtual void trackInPictures(vector<Mat> pictures) = 0;
	virtual void trackInVideo(String filename) = 0;
};
}
#endif /* OBJECTTRACKER_H_ */
