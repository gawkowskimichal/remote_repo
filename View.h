/*
 * View.h
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */
#include <gtkmm.h>
#include "Configuration.h"
#include <iostream>
#ifndef VIEW_H_
#define VIEW_H_

class View {
public:
	View(int argc, char *argv[], Configuration conf);
	int startView(int argc, char *argv[], Configuration conf);
	virtual ~View();
};

class FrmMain : public Gtk::Window{
protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Button *btnKalibracja;
    Gtk::Button *btnNagrywanie;
    Gtk::Button *btnKonfiguracja;
    Gtk::Button *btnExit;

public:
    FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor

protected:
    //signal handlers
    void on_btnKalibracja_clicked();
    void on_btnNagrywanie_clicked();
    void on_btnKonfiguracja_clicked();
    void on_btnExit_clicked();
};

#endif /* VIEW_H_ */
