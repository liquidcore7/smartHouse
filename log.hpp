//
// Created by liquidcore7 on 4/14/17.
//

#ifndef SMARTHOUSE_LOG_HPP
#define SMARTHOUSE_LOG_HPP

#include <string>
#include <iterator>
#include <ctime>
#include <cstring>
#include <iostream>
#include <sstream>

class log_service;

class log_message {
	const char* timestamp() const;
	char code;
	const char* text;
public:
	operator std::string() const;
	log_message(const std::string &, char, log_service &);
};

class log_service {
	std::ostream_iterator<std::string> out;
public:
	friend log_service& operator<<(log_service&, const log_message&);
	log_service() : out(std::ostream_iterator<std::string>(std::cout))
	{}
};

log_service& operator<<(log_service&, const log_message &);


#endif //SMARTHOUSE_LOG_HPP
