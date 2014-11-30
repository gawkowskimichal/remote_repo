/*
 * Program.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_
#include "Configuration.h"
#include "VideoManager.h"
#include "View.h"
#include "ConfReader.h"
#include <gtkmm.h>
#include <iostream>

using namespace TracingFramework;

class Program {
public:
	VideoManager * man;
	View * view;
	ConfReader * reader;
	Program(int argc, char *argv[]);
	void test_Video();
	void test_vector_capture();
	void test_VideoMult();
	void test_vector_capture_mult();
	void test_alvar_tracking();
	void test_alvar_tracking_mult();
	void test_ball_tracking();
	void test_ball_tracking_mult();
	void test_tracking_simultanous();
	void test_tracking_mult_simultanous();
	void test_epipolar();
	virtual ~Program();

};

#endif /* PROGRAM_H_ */
