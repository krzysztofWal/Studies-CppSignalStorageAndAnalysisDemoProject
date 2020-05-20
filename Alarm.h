/*
 * Alarm.h
 *
 *  Created on: 16 kwi 2020
 *      Author: student
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "Packet.h"

template<class T>
class Alarm: public Packet {
protected:
	int channelNr;
	T threshold;
	int direction;
public:
	Alarm();
	Alarm(std::string device, std::string description, long date, int channelNr, T threshold, int direction);
	virtual ~Alarm() {}

	virtual std::string toString();
	virtual void fun() override {};
};

template<class T> Alarm<T>::Alarm()
 : Packet{}, channelNr{0}, threshold{static_cast<T>(0)}, direction{0}{
}

template<class T> Alarm<T>::Alarm(std::string device, std::string description, long date, int channelNr, T threshold, int direction)
 : Packet{device, description, date}, channelNr{channelNr}, threshold{threshold}, direction{direction}{
}

template<class T> std::string Alarm<T>::toString(){
	return (Packet::toString() + "/" + std::to_string(channelNr) + "/" + std::to_string(threshold) + "/" + std::to_string(direction));
}

#endif /* ALARM_H_ */
