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
#include "AlvarObjectTracker.h"
#include "BallObjectTracker.h"
#include "Configuration.h"
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace cv;
namespace TracingFramework {
class VideoManager {
public:
	VideoConnector * connector;
	ObjectTracker *tracker;
	vector<ObjectTracker*> trackers;
	Mat Q;
	boost::thread capturing_thread;
	boost::thread tracking_thread;
	int ReaderCounter;
	VideoManager(Configuration conf);
	virtual ~VideoManager();

	void initTrackers(Configuration conf);
	void initQMatrix(Configuration conf, bool reinit);

	void TrackFromFiles(Configuration conf);
	void TrackMultipleFromFiles(Configuration conf);
	void TrackMultipleFromFilesTriangulate(Configuration conf);

	void CaptureAndTrack(Configuration conf);
	void CaptureAndTrackMultiple(Configuration conf);
	void CaptureAndTriangulate(Configuration conf);

	void TrackInPicture(Mat picture, String time);
	void TrackInPictures(vector<std::pair<Mat,String>> pictures);
	void TrackInVideo(String fileName);

	void CaptureFrom( int i );
	void CaptureMultiple();
	void CaptureSnapshotFrom(int i);
	void CaptureSnapshotFromBoth();
	void getCalibrationMaterial(Configuration conf, int i);
	void getCalibrationMultipleMaterial(Configuration conf);
	void CaptureToFiles(Configuration conf);
	void CaptureMultipleToFiles(Configuration conf);


	void captureVideoToFileFromCam(int i, Configuration conf);
	void captureVideoToFilesFromCams(Configuration conf);

	Mat readImageFromFile(String path);

	vector<std::pair<Mat,String>> captureToMemory(int i, Configuration conf);
	vector<vector<std::pair<Mat,String>>> captureToMemoryMultiple(Configuration conf);

	Point3f triangulatePointsFromMultipleCameras(Configuration conf, int cameraOneIndex, int cameraTwoIndex, Point2f firstImgPoint, Point2f secondImgPoint);
	void shutdown();
	void test_epipolar(Configuration conf);
};
}
#endif /* VIDEOMANAGER_H_ */
