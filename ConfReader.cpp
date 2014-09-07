/*
 * ConfReader.cpp
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#include "ConfReader.h"

ConfReader::ConfReader() {
	readConf();
}

ConfReader::~ConfReader() {
	// TODO Auto-generated destructor stub
}

void ConfReader::readConf(){
	std::string s, key, value;
	std::ifstream f("settings.conf");
	while (std::getline(f,s)){
		std::string::size_type begin = s.find_first_not_of(" \f\t\v");
		if (begin == std::string::npos) continue;
		if (std::string( "#;" ).find(s[begin]) != std::string::npos) continue;
		std::string::size_type end = s.find('=',begin);
		key = s.substr(begin,end-begin);
		key.erase(key.find_last_not_of(" \f\t\v")+1);
		if(key.empty()) continue;
		begin = s.find_first_not_of(" \f\t\v\n\r", end + 1);
		end = s.find_last_not_of(" \f\t\v\n\r",1);
		value = s.substr(begin,end-begin);
		conf.setValueOfKey(key,value);
	}
	f.close();
}
