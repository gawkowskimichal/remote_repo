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
	virtual String trackInPicture(Mat picture, String time) = 0;
	virtual vector<String> trackInPictures(vector<std::pair<Mat,String>> pictures) = 0;
	virtual vector<String> trackInVideo(String filename) = 0;
	virtual vector<String> getTrack(vector<std::pair<Mat,String>> pictures) = 0;
	virtual vector<String> getTrack(String filename) = 0;
	virtual void saveTrackToFile(vector<String> pos, String filename) = 0;
	Point3f getPositionDiff(Point3f start, Point3f end);
	double getTimeDiff(String start, String stop);
	time_t getTimeFromString(String time_string);
	Point3f getSpeed(Point3f start_pos, String start_time, Point3f stop_pos, String stop_time);
};
}
#endif /* OBJECTTRACKER_H_ */
