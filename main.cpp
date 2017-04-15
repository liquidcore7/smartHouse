#include "server/smartHouseApi.hpp"

PinCtrl board = {{2, false}, {3, false}, {4, true}};
shServer serv(board);

int main() {
	std::cout << "SmartHouse server v1.0\n";
	serv.start();
    return 0;
}