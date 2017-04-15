//
// Created by liquidcore7 on 4/14/17.
//

#include "log.hpp"

const char *log_message::timestamp() const {
	time_t currTime(NULL);
	time(&currTime);
	char * tmp = ctime(&currTime);
	tmp[strlen(tmp) - 1] = '\0';
	return tmp;
}

log_message::operator std::string() const {
	std::stringstream txt;
	txt << timestamp() << " [" << code << "] : " << text << '\n';
	return txt.str();
}

log_service& operator<<(log_service& ls, const log_message &lm)
{
	*(ls.out++) = static_cast<std::string> (lm);
}

log_message::log_message(const std::string &msg, char _cde, log_service &logger)
: code(_cde), text(msg.c_str())
{
	logger << *this;
}



