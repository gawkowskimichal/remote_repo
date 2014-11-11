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
	//view = new View(argc, argv,reader->conf, man);
}

Program::~Program() {
}

void Program::test_Video(){
	this->man->captureVideoToFileFromCam(0,this->reader->conf);
};

void Program::test_VideoMult(){
	this->man->captureVideoToFilesFromCams(this->reader->conf);
};

void Program::test_vector_capture(){
	vector<std::pair<Mat,String>> res = this->man->captureToMemory(0,this->reader->conf);
	cout << res.size() << endl;
};

void Program::test_vector_capture_mult(){
	vector<vector<std::pair<Mat,String>>> res = this->man->captureToMemoryMultiple(this->reader->conf);
	cout << res.size() << endl;
	for (int i = 0; i < res.size(); i++){
		cout <<  res[i][0].second << endl;
	}
};

void Program::test_alvar_tracking(){
	vector<std::pair<Mat,String>> res = this->man->captureToMemory(0,this->reader->conf);
	cout << res.size() << endl;
	this->man->tracker = new AlvarObjectTracker(this->reader->conf, "./Debug/out_camera_data4.xml");
	cout << "After constructor" << endl;
	vector<String> pos = this->man->tracker->trackInPictures(res);
	cout << "After tracking" << endl;
	for (int i = 0; i < pos.size(); i++){
		cout << pos[i] << endl;
	}
};

void Program::test_alvar_tracking_mult(){

};

void Program::test_ball_tracking(){
	vector<std::pair<Mat,String>> res = this->man->captureToMemory(0,this->reader->conf);
	cout << res.size() << endl;
	this->man->tracker = new BallObjectTracker(this->reader->conf, "./Debug/out_camera_data4.xml");
	cout << "After constructor" << endl;
	vector<String> pos = this->man->tracker->trackInPictures(res);
	cout << "After tracking" << endl;
	for (int i = 0; i < pos.size(); i++){
		cout << pos[i] << endl;
	}
};

void Program::test_ball_tracking_mult(){

};


void Program::test_tracking_simultanous(){
	this->man->CaptureAndTrack(this->reader->conf);
};

void Program::test_tracking_mult_simultanous(){
	this->man->CaptureAndTrackMultiple(this->reader->conf);
};

int main(int argc, char *argv[]){
	Program *p = new Program(argc,argv);
	//p->test_Video();
	//p->test_VideoMult();
	//p->test_vector_capture();
	//p->test_vector_capture_mult();
	//p->test_alvar_tracking();
//	p->test_alvar_tracking_mult();
	//p->test_ball_tracking();
//	p->test_ball_tracking_mult();
	p->test_tracking_simultanous();
	//p->man->getCalibrationMaterial();
	//p->man->getCalibrationMultipleMaterial(p->reader->conf);
	//p->man->shutdown();
	//p->man->CaptureFrom(0);
	//p->man->CaptureFrom(1);
	//p->man->CaptureMultiple();
	//p->man->CaptureFromBoth();
return 1;
}
