/*
 * Camera.cpp
 *
 *  Created on: 21 Sep 2014
 *      Author: mgw
 */

#include "Camera.h"

using namespace std;
using namespace cv;

Camera::Camera(int i) {
	this->index = i;
	cam = cvCaptureFromCAM( index );
}

void Camera::setCaptureOption(int option, double value){
	cvSetCaptureProperty(cam,option,value);
};

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

