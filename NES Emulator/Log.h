#pragma once

#include <string>
#include <fstream>
#include <ostream>
#include <iostream>

// Can have 3 States:
//		std::cout	Log() or Log("");
//		file out	Log("filename")
//		OFF			SetState(false);
class Log {
private:
	std::ostream* out;
	std::ofstream* f;
	bool fileMode;
	int writeCounter;
	bool secondFile = false;
	std::string filename;
	bool isOn;

	void IncWriteCounter() { writeCounter++; }
	void SetupCout() {
		out = &std::cout; 
		fileMode = false;
		isOn = true;
	}
	void SetupFile(std::string _filename) {
		if (_filename != "") {
			filename = _filename;
			fileMode = true;
			writeCounter = 0;
			f = new std::ofstream;
			f->exceptions(std::ifstream::failbit | std::ifstream::badbit);
			f->open(filename + ".txt", std::ios::out | std::ios::trunc);
			out = f;
			isOn = true;
		}
		else {
			SetupCout();
		}
	}

public:
	template<typename T>
	friend Log& operator<<(Log& log, T const& data);

	void SetState(bool toOn) {
		isOn = toOn;
	}
	void flush() {
		if (isOn) {
			out->flush();
		}
	}

	Log() { 
		SetupCout();
	}
	Log(std::string _filename) {
		SetupFile(_filename);
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
	if (!log.isOn) return log;
	(*log.out) << data;
	if (log.fileMode) log.writeCounter++;
	if (log.writeCounter >= 2000000) {
		log.writeCounter = 0;
		log.f->close();
		if (!log.secondFile)
			log.f->open(log.filename + " (1).txt", std::ios::out | std::ios::trunc);
		else
			log.f->open(log.filename + ".txt", std::ios::out | std::ios::trunc);
		log.secondFile = !log.secondFile;
	}
	return log;
}