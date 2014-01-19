/*
 * Program.h
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_
#include "VideoManager.h"

class Program {
public:
	VideoManager * man;
	Program();
	virtual ~Program();

};

#endif /* PROGRAM_H_ */
