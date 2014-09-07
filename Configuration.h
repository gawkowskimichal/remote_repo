/*
 * Conf.h
 *
 *  Created on: 7 Sep 2014
 *      Author: mgw
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>
#include <map>
#include <string>

class Configuration {
public:
	struct data {
		std::map <std::string,std::string> internalMap;
			bool isKey(const std::string& s) const {
				return internalMap.count(s) != 0;
			}
		} conf;
	Configuration();
	virtual ~Configuration();
	std::string getValueByKey(std::string key);
	void setValueOfKey(std::string key, std::string value);
};

#endif /* CONF_H_ */
