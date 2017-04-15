//
// Created by liquidcore7 on 4/2/17.
//

#include "PinCtrl.hpp"


Pin::Pin(int idx, direction _direction) {
	if (std::find(avaliablePins, avaliablePins + 16, idx) == avaliablePins + 16)
		throw std::range_error("GPIO index unavaliable");
	std::ofstream setup("/sys/class/gpio/export");
	setup << idx;
	setup.close();
	setup.open("/sys/class/gpio/gpio" + std::to_string(idx) + "/direction");
	setup << ((_direction == OUT) ? "out" : "in");
	setup.close();
	setup.open("/sys/class/gpio/gpio" + std::to_string(idx) + "/value");
	setup << 1;
	setup.close();
	id = idx;
	dir = _direction;
}

void Pin::set(bool state) {
	std::ofstream st("/sys/class/gpio/gpio" + std::to_string(id) + "/value");
	st << !state;
	st.close();
	val = state;
}

bool Pin::getState() {
	std::ifstream gt("/sys/class/gpio/gpio" + std::to_string(id) + "/value");
	bool state;
	gt >> state;
	gt.close();
	return state;
}

Pin::~Pin() {
	std::ofstream unexp("/sys/class/gpio/unexport");
	unexp << id;
	unexp.close();
}

PinCtrl::PinCtrl(const std::initializer_list<std::pair<int, bool> > &mapping) {
		for (const auto &init : mapping) {
			std::unique_ptr<Pin> pin(new Pin(init.first));
			pin->set(init.second);
			std::pair<int, std::unique_ptr<Pin> > tmpPr(init.first, std::move(pin));
			pinMap.insert(std::move(tmpPr));
		}
}

void PinCtrl::set_all(bool val) {
	for (const auto &pn : pinMap)
		pn.second->set(val);
}

std::unique_ptr<Pin>& PinCtrl::getPin(int idx) {
	auto pos = pinMap.find(idx);
	if (pos == pinMap.end()) {
		std::unique_ptr<Pin> tmpPin(new Pin(idx));
		std::pair<int, std::unique_ptr<Pin> > tmpPr(idx, std::move(tmpPin));
		pos = pinMap.insert(std::move(tmpPr)).first;
	}
	return pos->second;
}

