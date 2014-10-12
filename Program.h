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
	virtual ~Program();

};

#endif /* PROGRAM_H_ */
