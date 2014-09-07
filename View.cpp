/*
 * View.cpp
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#include "View.h"
#include <iostream>

View::View(int argc, char *argv[]) {
	startView(argc,argv);
}

int View::startView(int argc, char *argv[]){
    Gtk::Main kit(argc, argv);
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
      try
      {
        refBuilder->add_from_file("GladeView.glade");
      }
      catch(const Glib::FileError& ex)
      {
       // std::out << "FileError: " << ex.what() << std::endl;
      }
      catch(const Gtk::BuilderError& ex)
      {
        // std::out << "BuilderError: " << ex.what() << std::endl;
      }
      Gtk::Window window;
      Gtk::Main::run(window);

      return 0;
}

View::~View() {
	// TODO Auto-generated destructor stub
}

