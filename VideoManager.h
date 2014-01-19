/*
 * VideoManager.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */
#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "VideoConnector.h"


class VideoManager {
public:
	VideoConnector * cam1;
	VideoManager();
	virtual ~VideoManager();
	void CaptureFrom(int i);
};

#endif /* VIDEOMANAGER_H_ */
