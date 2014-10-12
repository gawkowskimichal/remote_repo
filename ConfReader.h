/*
 * ConfReader.h
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#ifndef CONFREADER_H_
#define CONFREADER_H_
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include "Configuration.h"
namespace TracingFramework {
class ConfReader {
public:
	Configuration conf;
	ConfReader();
	void readConf();
	virtual ~ConfReader();
};
}
#endif /* CONFREADER_H_ */
