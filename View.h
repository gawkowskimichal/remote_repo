/*
 * View.h
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */
#include <gtkmm.h>
#include <iostream>
#ifndef VIEW_H_
#define VIEW_H_

class View {
public:
	View(int argc, char *argv[]);
	int startView(int argc, char *argv[]);
	virtual ~View();
};

#endif /* VIEW_H_ */
