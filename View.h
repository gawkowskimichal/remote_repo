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

#endif /* VIEW_H_ */
