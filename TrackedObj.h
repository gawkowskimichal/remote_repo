/*
 * TrackedObj.h
 *
 *  Created on: 10 Jan 2015
 *      Author: mgw
 */

#ifndef TRACKEDOBJ_H_
#define TRACKEDOBJ_H_
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


namespace TracingFramework {

class TrackedObj {
public:
	int numberOfPoints;
	std::vector<std::pair<std::vector<Point3f>,string>> trace;
	TrackedObj();
	TrackedObj(int num);
	void setNumberOfPoints(int num);
	int getNumberOfPoints();
	void addPos(std::pair<std::vector<Point3f>,string> new_pos);
	std::pair<std::vector<Point3f>,string> getLastPosition();
	virtual ~TrackedObj();
};

} /* namespace TracingFramework */

#endif /* TRACKEDOBJ_H_ */
