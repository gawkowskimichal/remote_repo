/*
 * Program.cpp
 *
 *  Created on: 18 Jan 2014
 *      Author: mgw
 */

#include "Program.h"



Program::Program(int argc, char *argv[]) {
	man = new VideoManager();
	view = new View(argc, argv);
}

Program::~Program() {
}

int main(int argc, char *argv[]){
	Program *p = new Program(argc,argv);
	//p->man->CaptureFrom(0);
	//p->man->CaptureFrom(1);
	/*
    Gtk::Main kit(argc, argv);

    Gtk::Window window;

    Gtk::Main::run(window);
*/

	//p->man->CaptureMultiple();
	//p->man->CaptureFromBoth();
return 1;
}
