#pragma once

#include <string>
#include <fstream>
#include <ostream>
#include <iostream>

class Log {
private:
	std::ostream* out;
	std::ofstream* f;
	bool fileMode;
	int writeCounter;
	bool secondFile = false;
	std::string filename;

public:
	template<typename T>
	friend Log& operator<<(Log& log, T const& data);
	void IncWriteCounter() { writeCounter++; }

	Log() { 
		out = &std::cout; 
		fileMode = false;
	}
	Log(std::string _filename) {
		filename = _filename;
		fileMode = true;
		writeCounter = 0;
		f = new std::ofstream;
		f->exceptions(std::ifstream::failbit | std::ifstream::badbit);
		f->open(filename, std::ios::out | std::ios::trunc);
		out = f;
	}
	~Log() {
		if (fileMode && f->is_open()) {
			f->close();
			delete f;
		}
	}
};

template<typename T>
Log& operator<<(Log& log, T const& data) {
	(*log.out) << data;
	if (log.fileMode) log.writeCounter++;
	if (log.writeCounter >= 2000000) {
		log.writeCounter = 0;
		log.f->close();
		if (!log.secondFile)
			log.f->open(log.filename + "1.txt", std::ios::out | std::ios::trunc);
		else
			log.f->open(log.filename, std::ios::out | std::ios::trunc);
	}
	return log;
}