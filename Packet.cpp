/*
 * Packet.cpp
 *
 *  Created on: 16 kwi 2020
 *      Author: student
 */

#include "Packet.h"

Packet::Packet()
 : device{"Not given"}, description{"Not given"}, date{0} {
}

Packet::Packet(std::string device, std::string description, long date)
: device{device}, description{description}, date{date} {
}

std::string Packet::toString() {
	return (device + "/" + description + "/" + std::to_string(date));
}

void Packet::fun() {
}
