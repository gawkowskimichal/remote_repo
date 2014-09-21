/*
 * VideoManager.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */
#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "VideoConnector.h"
#include "Configuration.h"
#include <thread>

using namespace std;

class VideoManager {
public:
	VideoConnector * connector;
	VideoManager(Configuration conf);
	virtual ~VideoManager();
	void CaptureFrom( int i );
	void CaptureMultiple();
	void CaptureSnapshotFrom(int i);
	void CaptureSnapshotFromBoth();
	void getCalibrationMaterial(Configuration conf, int i);
	void getCalibrationMultipleMaterial(Configuration conf);
	void shutdown();
};

#endif /* VIDEOMANAGER_H_ */
