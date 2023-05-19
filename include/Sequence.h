/*
 * Sequence.h
 *
 *  Created on: 16 kwi 2020
 *      Author: student
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "Packet.h"
#include <array>

template <class T, int I>
class Sequence: public Packet {
protected:
	int channelNr;
	std::string unit;
	double resolution;
	std::array<T,I> buffer;
public:
	Sequence();
	Sequence(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution);
	Sequence(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, std::array<T,I> buffer);
	virtual ~Sequence() {}
	virtual std::string toString();
	int getChannelNr() {
		return channelNr;
	}
	std::string getUnit(){
		return unit;
	}
	double getResolution(){
		return resolution;
	}
	std::array<T,I> getBuffer() {
		return buffer;
	}
	std::string getDevice() {
		return Packet::device;
	}
	std::string getDescription() {
		return Packet::description;
	}
	long getDate() {
		return Packet::date;
	}
};

template <class T, int I> Sequence<T, I>::Sequence()
 : Packet{}, channelNr{0}, unit{"Not given"}, resolution{0.0}, buffer{std::array<T,I> {}} {
}

template <class T, int I> Sequence<T, I>::Sequence(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution)
 : Packet{device,description,date}, channelNr{channelNr}, unit{unit}, resolution{resolution},buffer{std::array<T,I> {}} {}

template <class T, int I> Sequence<T, I>::Sequence(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, std::array<T,I> buffer)
 : Packet{device,description,date}, channelNr{channelNr}, unit{unit}, resolution{resolution}, buffer{buffer} {
}

template <class T, int I> std::string Sequence<T, I>::toString() {
return (Packet::toString() + "/" + std::to_string(channelNr) + "/" + unit + "/" + std::to_string(resolution));
}


#endif /* SEQUENCE_H_ */

