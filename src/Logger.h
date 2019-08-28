#pragma once

#include <string>
#include <fstream>
#include <ostream>
#include <iostream>

class Logger {
private:
	std::ofstream f;
	std::ostream* out = nullptr;
	void CloseIfOpen() {
		if (f.is_open()) {
			f.flush();
			f.close();
		}
	}
	void SetOutFile(const std::string& filename) {
		CloseIfOpen();
		f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		f.open(filename, std::ios::out | std::ios::trunc);
		out = &f;
	}
public:
	template<typename T>
	Logger& operator<<(T const& data) {
		if (this->out != nullptr) *(this->out) << data;
		return *this;
	}
	void flush() {
		if (f.is_open())
			f.flush();
	}
	Logger() {}
	Logger(const std::string& filename) {
		SetOutFile(filename);
	}
	~Logger() {
		CloseIfOpen();
	}
};