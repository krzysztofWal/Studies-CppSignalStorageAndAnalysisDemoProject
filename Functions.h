/*
 * Functions.h
 *
 *  Created on: 4 maj 2020
 *      Author: student
 */
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "TimeHistory.h"
#include "ffft/Array.h"
#include "ffft/def.h"
#include "ffft/DynArray.h"
#include "ffft/FFTReal.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <array>
#include <vector>

template<class T, int I> std::array<T, I> Gauss(T mean, T deviation);
template<class T, int I> T RMS(const std::array<T, I>& data);
//template<class T, int I> std::vector<std::array<T, 2048>> FFT(std::array<T,I> data, bool isAbs);


template<class T, int I> std::array<T, I> Gauss(T mean, T deviation) {
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::normal_distribution<> d{ mean,deviation };

	std::array<T, I>* temp = new std::array<T, I>{};
	size_t n{ 0 };
	T tempNum{};
	while (n < I)
	{
		tempNum = d(gen);
		if (tempNum > mean - deviation && tempNum < mean + deviation) {
			temp->at(n) = tempNum;
			n++;
		}
	}
	return *temp;
}

template<class T, int I> T RMS(const std::array<T, I>& data) {
	T temp{};
	for (const T& i : data) {
		temp += i * i;
	}
	return sqrt(temp / I);
}

template<class T, int I> std::vector<std::array<T, 2048>> FFT(std::array<T, I> data, bool isAbs) {
	std::array<T, 2048> dataPrepared;
	for (size_t i{ 0 }; i < 2048; i++) {
		dataPrepared.at(i) = data.at(i);
	}
	T f[2048];
	ffft::FFTReal<T> fftObject(2048);
	fftObject.do_fft(f, &dataPrepared[0]);
	std::array<T, 2048> fReal;
	std::array<T, 2048> fImaginary;
	std::array<T, 2048> abs;
	std::vector<std::array<T, 2048>> temp;
	size_t length = 2048;

	for (size_t i{ 0 }; i < length / 2; i++) { //do I/2 -1
		fReal.at(i) = f[i];
		//std::cout << 1 << std::endl;
	}
	for (size_t i{ 1 }; i <= length / 2; i++) { //od 1 do I/2 tylko na odwrot
		fReal.at(length - i) = f[i];
		//std::cout << 2 << std::endl;
	}

	fImaginary.at(0) = 0.0;
	for (size_t i{ length / 2 + 1 }; i < length; i++) {
		fImaginary.at(i - length / 2) = f[i];
		//std::cout << 3 << std::endl;
	}
	fImaginary.at(length / 2) = 0.0;
	for (size_t i{ 2 }; i <= length / 2; i++) {
		fImaginary.at(i + length / 2 - 1) = f[length - i + 1];
		//	std::cout << 4 << std::endl;
	}

	for (size_t i{ 0 }; i < length; i++) {
		abs.at(i) = sqrt(fImaginary.at(i) * fImaginary.at(i) + fReal.at(i) * fReal.at(i));
	}

	if (isAbs)
		temp.emplace_back(abs);
	else {
		temp.emplace_back(fReal);
		temp.emplace_back(fImaginary);
	}

	return temp;
}

#endif /* FUNCTIONS_H_ */