/*
 * ObjectTracker.cpp
 *
 *  Created on: 5 Oct 2014
 *      Author: mgw
 */

#include "ObjectTracker.h"
using namespace TracingFramework;

ObjectTracker::ObjectTracker() {
	// TODO Auto-generated constructor stub

}

ObjectTracker::~ObjectTracker() {
	// TODO Auto-generated destructor stub
}

Point3f ObjectTracker::getPositionDiff(Point3f start, Point3f end){
	Point3f res(0,0,0);
	res.x = abs(start.x - end.x);
	res.y = abs(start.y - end.y);
	res.z = abs(start.z - end.z);
	return res;
}

time_t ObjectTracker::getTimeFromString(String time_string){
	struct tm tm;
	strptime(time_string.c_str(), "%Y%b%d%H%M%S", &tm);
	return mktime(&tm);
}

double ObjectTracker::getTimeDiff(String start, String stop){
	time_t start_time = getTimeFromString(start);
	time_t end_time = getTimeFromString(stop);
	return difftime(end_time, start_time);
};
