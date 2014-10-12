/*
 * Conf.cpp
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#include "Configuration.h"
using namespace TracingFramework;

Configuration::Configuration() {
}

Configuration::~Configuration() {
}

std::string Configuration::getValueByKey(std::string key){
	if (conf.isKey(key)){
		return conf.internalMap.at(key);
	} else {
		return std::string("");
	}
}

void Configuration::setValueOfKey(std::string key, std::string value){
	conf.internalMap[key] = value;
}
