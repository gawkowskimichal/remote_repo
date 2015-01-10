/*
 * TrackedObj.cpp
 *
 *  Created on: 10 Jan 2015
 *      Author: mgw
 */

#include "TrackedObj.h"

namespace TracingFramework {

TrackedObj::TrackedObj() {
	numberOfPoints = 0;
}

TrackedObj::TrackedObj(int num) {
	numberOfPoints = num;
}

TrackedObj::~TrackedObj(){};

void TrackedObj::setNumberOfPoints(int num){
	this->numberOfPoints = num;
}

int TrackedObj::getNumberOfPoints(){
	return numberOfPoints;
}

void TrackedObj::addPos(std::pair<std::vector<Point3f>,string> new_pos){
	trace.push_back(new_pos);
};

std::pair<vector<Point3f>,string> TrackedObj::getLastPosition(){
	std::pair<vector<Point3f>,string> pair;
	if (trace.size() > 0){
		return trace.at(trace.size() - 1);
	} else {
		return pair;
	}
};

} /* namespace TracingFramework */
