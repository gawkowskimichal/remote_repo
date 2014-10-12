/*
 * BallObjectTracker.cpp
 *
 *  Created on: 12 Oct 2014
 *      Author: mgw
 */

#include "BallObjectTracker.h"

namespace TracingFramework {

BallObjectTracker::BallObjectTracker(Configuration conf, String calibfn) {
	calibrationFile = calibfn;
	config = &conf;
}

BallObjectTracker::~BallObjectTracker() {
	// TODO Auto-generated destructor stub
}

void BallObjectTracker::trackInPicture(Mat picture){

};
void BallObjectTracker::trackInPictures(vector<Mat> pictures){

};
void BallObjectTracker::trackInVideo(String filename){

};
} /* namespace TracingFramework */
