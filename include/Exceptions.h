/*
 * Exceptions.h
 *
 *  Created on: 8 maj 2020
 *      Author: student
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <eh.h>

class FilesNotOpened : public std::runtime_error {
public:
	FilesNotOpened() : std::runtime_error {"Files didn't open properly"} {};
};

class TooMuchTime : public std::runtime_error {
public:
	TooMuchTime() : std::runtime_error{"Operation took too long"} {};
};

class SystemException {
private:
	unsigned int nSE;
public:
	SystemException(unsigned int n) : nSE(n) {}
	~SystemException() {}
	unsigned int seNumber() {
		return nSE;
	}
};

void ExceptionHandler(unsigned int u, EXCEPTION_POINTERS* pExp) {
	throw new SystemException(u);
}

void checkForTime(time_t& secondsElapsed, const time_t& plannedTime) {
	if (time(NULL) - secondsElapsed > plannedTime) {
		throw TooMuchTime();
	}
	secondsElapsed = time(NULL);
}


#endif /* EXCEPTIONS_H_ */
