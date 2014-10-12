/*
 * View.cpp
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#include "View.h"
#include <iostream>

using namespace TracingFramework;

View::View(int argc, char *argv[], Configuration conf, VideoManager *manager) {
	man = manager;
	startView(argc,argv,conf);
}

int View::startView(int argc, char *argv[], Configuration conf){
	std::string GladeFile = conf.getValueByKey("gladeFile");
    Gtk::Main kit(argc, argv);
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(GladeFile);
    FrmMain *frm = 0;
    refBuilder->get_widget_derived("menu", frm);
    frm->initManager(man,conf);
    kit.run(*frm);
    return 0;
}

View::~View() {
	// TODO Auto-generated destructor stub
}


FrmMain::FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade){

    builder->get_widget("btnKalibracja", btnKalibracja);
    builder->get_widget("btnNagrywanie", btnNagrywanie);
    builder->get_widget("btnKonfiguracja", btnKonfiguracja);
    builder->get_widget("btnExit", btnExit);

    btnKalibracja->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnKalibracja_clicked));
    btnNagrywanie->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnNagrywanie_clicked));
    btnKonfiguracja->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnKonfiguracja_clicked));
    btnExit->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnExit_clicked));
}

void FrmMain::initManager(VideoManager *man, Configuration conf){
	internalManager = man;
	internalConfiguration = conf;
}

void FrmMain::on_btnExit_clicked(){
	Gtk::Main::quit();
}

void FrmMain::on_btnNagrywanie_clicked(){

}


void FrmMain::on_btnKonfiguracja_clicked(){

}


void FrmMain::on_btnKalibracja_clicked(){
	internalManager->getCalibrationMultipleMaterial(internalConfiguration);
}

