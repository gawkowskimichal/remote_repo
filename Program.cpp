/*
 * Program.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "Program.h"



Program::Program(int argc, char *argv[]) {
	reader = new ConfReader();
	reader->readConf();
	man = new VideoManager(reader->conf);
	view = new View(argc, argv,reader->conf, man);
}

Program::~Program() {
}

int main(int argc, char *argv[]){
	Program *p = new Program(argc,argv);
	//p->man->getCalibrationMaterial(p->reader->conf,0);
	//p->man->getCalibrationMultipleMaterial(p->reader->conf);
	//p->man->shutdown();
	//p->man->CaptureFrom(0);
	//p->man->CaptureFrom(1);
	//p->man->CaptureMultiple();
	//p->man->CaptureFromBoth();
return 1;
}
