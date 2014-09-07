/*
 * View.cpp
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#include "View.h"
#include <iostream>

View::View(int argc, char *argv[], Configuration conf) {
	startView(argc,argv,conf);
}

int View::startView(int argc, char *argv[], Configuration conf){
	std::string GladeFile = conf.getValueByKey("gladeFile");
    Gtk::Main kit(argc, argv);
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
      try
      {
        refBuilder->add_from_file(GladeFile);
      }
      catch(const Glib::FileError& ex)
      {
    	  std::cout << "FileError: " << ex.what() << std::endl;
      }
      catch(const Gtk::BuilderError& ex)
      {
    	  std::cout << "BuilderError: " << ex.what() << std::endl;
      }
      Gtk::Window window;
      Gtk::Main::run(window);

      return 0;
}

View::~View() {
	// TODO Auto-generated destructor stub
}

