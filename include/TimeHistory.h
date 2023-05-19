/*
 * TimeHistory.h
 *
 *  Created on: 16 kwi 2020
 *      Author: student
 */

#ifndef TIMEHISTORY_H_
#define TIMEHISTORY_H_

#include "Sequence.h"
#include "Functions.h"
#include <array>
#include <iostream>
#include <fstream>

template<class T, int f, int t>
class TimeHistory: public Sequence<T, f*t> {
	double sensitivity;
public:
	TimeHistory();
	TimeHistory(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, double sensitivity);
	TimeHistory(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, std::array<T,f*t> buffer, double sensitivity);
	virtual ~TimeHistory() {}

//	void printData();
	void simulate(T mean, T deviation);
	virtual std::string toString();
	virtual void fun() override {}
	T calculateRms();

	TimeHistory<T,f,t> &operator=(const TimeHistory<T,f,t> &rhs);
	TimeHistory<T,f,t> operator+(TimeHistory<T,f,t> &rhs) const;
	TimeHistory<T,f,t> operator/(TimeHistory<T,f,t> &rhs) const;

	friend std::ofstream& operator << (std::ofstream &os, const TimeHistory<T,f,t> &obj) {
	for (size_t i{0}; i < (obj.buffer.size()) ; i++) {
		os << /*((i+1)/static_cast<double>(f)) << " " <<*/ obj.buffer.at(i) << '\n';
	}
	return os;
	}
};

template<class T,  int f, int t> TimeHistory<T, f, t>::TimeHistory()
 : Sequence<T, f*t>{}, sensitivity{0.0}{
}

 template<class T,  int f, int t> TimeHistory<T, f, t>::TimeHistory(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, double sensitivity)
  : Sequence<T, f*t>{device, description, date, channelNr, unit, resolution}, sensitivity{sensitivity} {}

template<class T,  int f, int t> TimeHistory<T, f, t>::TimeHistory(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, std::array<T,f*t> buffer, double sensitivity)
 : Sequence<T, f*t>{device, description, date, channelNr, unit, resolution, buffer}, sensitivity{sensitivity} {
}

template<class T,  int f, int t> std::string TimeHistory<T, f, t>::toString() {
	return (Sequence<T,f*t>::toString() + "/" + std::to_string(sensitivity));
}

template<class T,  int f, int t> void TimeHistory<T, f, t>::simulate(T mean, T deviation) {
	Sequence<T, f*t>::buffer = Gauss<T, f*t>(mean, deviation);
}
template<class T,  int f, int t> T TimeHistory<T, f, t>::calculateRms() {
	return RMS<T,f*t>(Sequence<T,f*t>::buffer);
}

template<class T,  int f, int t>  TimeHistory<T, f, t> &TimeHistory<T,f,t>::operator=(const TimeHistory<T,f,t> &rhs){
	Sequence<T,f*t>::buffer = rhs.buffer;
	return *this;
}

template<class T,  int f, int t>  TimeHistory<T, f, t> TimeHistory<T,f,t>::operator+( TimeHistory<T,f,t> &rhs) const{
	TimeHistory<T, f, t> temp;
	for (int i{0}; i<f*t; i++) {
		temp.buffer.at(i) = Sequence<T, f*t>::buffer.at(i) + rhs.buffer.at(i);
	}
	return temp;
}

template<class T,  int f, int t>  TimeHistory<T, f, t> TimeHistory<T,f,t>::operator/( TimeHistory<T,f,t> &rhs) const{
	TimeHistory<T, f, t> temp;
	for (int i{0}; i<f*t; i++) {
		temp.buffer.at(i) = Sequence<T, f*t>::buffer.at(i) / rhs.buffer.at(i);
	}
	return temp;
}

#endif /* TIMEHISTORY_H_ */
