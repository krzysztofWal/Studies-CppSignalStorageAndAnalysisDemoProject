/*
 * Spectrum.h
 *
 *  Created on: 16 kwi 2020
 *      Author: student
 */

#ifndef SPECTRUM_H_
#define SPECTRUM_H_

#include "Sequence.h"
#include "Functions.h"
#include "Complex.h"
#include <vector>
#include <cmath>

enum scaleType {
	logarythmic,
	linear
};

template<class T, int f, int t>
class Spectrum: public Sequence<T,f*t> {
protected:
	scaleType scaling;
public:
	Spectrum();
	Spectrum(TimeHistory<T,f,t> timeSequenceObj, scaleType scaling);
	Spectrum(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, std::array<T,f*t> buffer, scaleType scaling);
	virtual ~Spectrum() {}

	virtual void printSpectrum() {}
	virtual std::string toString();
	virtual void fun() override {};
};

// --- SPECTRUM ABS
template<class T, int f, int t>
class SpectrumAbs: public Spectrum<T,f,t> {
protected:
	std::vector<std::array<T,2048>> fourierVec;
public:
	SpectrumAbs(TimeHistory<T,f,t> timeSequenceObj, scaleType scaling);
	virtual ~SpectrumAbs() {}
	friend std::ofstream& operator << (std::ofstream &os, const SpectrumAbs<T,f,t> &obj) {
		for (const auto &el : obj.fourierVec) {
			for (auto i : el) {
	 			os << " " << i << '\n';
			}
	 	}
		return os;
	}
};

//---SPECTRUM COMPLEX
template<class T, int f, int t>
class SpectrumComplex: public Spectrum<T,f,t> {
protected:
	std::vector<std::array<T,2048>> fourierVec;
public:
	SpectrumComplex(TimeHistory<T,f,t> timeSequenceObj, scaleType scaling);
	virtual ~SpectrumComplex() {}

	friend std::ofstream& operator << (std::ofstream &os, const SpectrumComplex<T,f,t> &obj) {
		for(size_t i{0}; i<obj.fourierVec.at(1).size()/2 ; i++) {
			os << obj.fourierVec.at(0).at(i);
			obj.fourierVec.at(1).at(i) >= 0 ? (os << " + " << obj.fourierVec.at(1).at(i)) : (os << " - " << -obj.fourierVec.at(1).at(i));
		  	os << "i" << "\n";
		}
		for(size_t i{1}; i<obj.fourierVec.at(1).size()/2 - 1; i++) {
			os << obj.fourierVec.at(0).at(obj.fourierVec.at(1).size()/2-i - 1);
			obj.fourierVec.at(1).at(obj.fourierVec.at(1).size()/2 - i - 1) >= 0 ?
			(os << " + " << obj.fourierVec.at(1).at(obj.fourierVec.at(1).size()/2-i - 1)) :
			(os << " - " << -obj.fourierVec.at(1).at(obj.fourierVec.at(1).size()/2-i - 1));
			os << "i" << "\n";
		   }
		return os;
	}
};

template<class T, int f, int t> Spectrum<T,f,t>::Spectrum()
 : Sequence<T,f*t>{}, scaling{linear} {
}

 template<class T, int f, int t> Spectrum<T,f,t>::Spectrum(std::string device, std::string description, long date, int channelNr, std::string unit, double resolution, std::array<T,f*t> buffer, scaleType scaling)
  : Sequence<T,f*t>{device,description,date, channelNr, unit, resolution, buffer}, scaling{scaling} {
 }

template<class T, int f, int t> std::string Spectrum<T, f, t>::toString() {
	std::string skala = scaling==linear ? "linear": "logarythmic";
	return (Sequence<T,f*t>::toString() + "/" + skala);
}

//spectrum
template<class T, int f, int t> Spectrum<T,f,t>::Spectrum(TimeHistory<T,f,t> timeSequenceObj, scaleType scaling)
 : Sequence<T,f*t>{timeSequenceObj.getDevice(), timeSequenceObj.getDescription(), timeSequenceObj.getDate(),
	timeSequenceObj.getChannelNr(), timeSequenceObj.getUnit(), timeSequenceObj.getResolution(), timeSequenceObj.getBuffer()},
	scaling{scaling} {}

// spectrum complex
template<class T, int f, int t> SpectrumComplex<T,f,t>::SpectrumComplex(TimeHistory<T,f,t> timeSequenceObj, scaleType scaling)
 : Spectrum<T,f,t>{timeSequenceObj, scaling}, fourierVec{FFT<T, f*t>(timeSequenceObj.getBuffer(),false)} {}

 // spectrum abs
template<class T, int f, int t> SpectrumAbs<T,f,t>::SpectrumAbs(TimeHistory<T,f,t> timeSequenceObj, scaleType scaling)
 : Spectrum<T,f,t>{timeSequenceObj, scaling}, fourierVec{FFT<T, f*t>(timeSequenceObj.getBuffer(),true)}
 {}

#endif /* SPECTRUM_H_ */
