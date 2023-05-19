/*
 * Packet.h
 *
 *  Created on: 16 kwi 2020
 *      Author: student
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <string>

class Packet {
protected:
	std::string device;
	std::string description;
	long date;
public:
	Packet();
	Packet(std::string device, std::string description, long date);
	virtual ~Packet() {}

	virtual std::string toString();
	virtual void fun() = 0;
};

#endif /* PACKET_H_ */
