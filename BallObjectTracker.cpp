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

String BallObjectTracker::trackInPicture(Mat picture, String time){
	String result;
	return result;
};
vector<String> BallObjectTracker::trackInPictures(vector<std::pair<Mat,String>> pictures){
	vector<String> result;
	String partial;
	for (std::vector<std::pair<Mat,String>>::iterator it = pictures.begin(); it != pictures.end(); ++it){
		partial = trackInPicture((*it).first,(*it).second);
		result.push_back(partial);
	}
	return result;
};

vector<String> BallObjectTracker::trackInVideo(String filename){

};

void BallObjectTracker::saveTrackToFile(vector<String> pos, String filename){

};

} /* namespace TracingFramework */
