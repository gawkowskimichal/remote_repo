/*
 * VideoManager.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */
#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "VideoConnector.h"
#include "ObjectTracker.h"
#include "Configuration.h"
#include <thread>

using namespace std;
using namespace cv;
namespace TracingFramework {
class VideoManager {
public:
	VideoConnector * connector;
	ObjectTracker *tracker;
	VideoManager(Configuration conf);
	virtual ~VideoManager();
	void TrackInPicture(Mat picture);
	void TrackInPictures(vector<Mat> pictures);
	void TrackInVideo(String fileName);
	void CaptureFrom( int i );
	void CaptureMultiple();
	void CaptureSnapshotFrom(int i);
	void CaptureSnapshotFromBoth();
	void getCalibrationMaterial(Configuration conf, int i);
	void getCalibrationMultipleMaterial(Configuration conf);
	void shutdown();
};
}
#endif /* VIDEOMANAGER_H_ */
