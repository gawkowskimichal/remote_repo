/*
 * Program.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "Program.h"

Program::Program() {
	man = new VideoManager();
}

Program::~Program() {
}

int main(){
	Program *p = new Program();
	p->man->CaptureFrom(0);
	p->man->CaptureFrom(1);
	//p->man->CaptureMultiple();
	//p->man->CaptureFromBoth();
return 1;
}
