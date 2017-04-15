//
// Created by liquidcore7 on 4/2/17.
//

#ifndef SMARTHOUSE_PINCTRL_HPP
#define SMARTHOUSE_PINCTRL_HPP

#include <fstream>
#include <algorithm>
#include <map>
#include <memory>

enum direction {IN, OUT};
constexpr int avaliablePins[] = {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27};

class Pin {
	int id;
	direction dir;
	bool val;
public:
	Pin(int, direction = OUT);
	Pin(const Pin&) = delete;
	Pin& operator=(const Pin&) = delete;
	void set(bool);
	bool getState();
	~Pin();
};

class PinCtrl {
	std::map<int, std::unique_ptr<Pin> > pinMap;
public:
	PinCtrl() = default;
	PinCtrl(const std::initializer_list<std::pair<int, bool> >&);
	void set_all(bool);
	std::unique_ptr<Pin>& getPin(int);
	~PinCtrl() = default;
};


#endif //SMARTHOUSE_PINCTRL_HPP
