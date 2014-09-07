/*
 * Program.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_
#include "VideoManager.h"
#include "View.h"
#include <gtkmm.h>
#include <iostream>

class Program {
public:
	VideoManager * man;
	View * view;
	Program(int argc, char *argv[]);
	virtual ~Program();

};

#endif /* PROGRAM_H_ */
