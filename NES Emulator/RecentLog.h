#ifndef RECENTLOG_H
#define RECENTLOG_H

#include "Log.h"

class RecentLog : public Log {

};

lass Log{
private:
	std::ostream* out;
	std::ofstream* f;
	bool fileMode;
	int writeCounter;
	bool secondFile = false;
	std::string filename;

public:
	template<typename T>
	friend RecentLog& operator<<(RecentLog& rlog, T const& data)
	void IncWriteCounter() { writeCounter++; }

	RecentLog() {
		out = &std::cout;
		fileMode = false;
	}
	RecentLog(std::string _filename) {
		filename = _filename;
		fileMode = true;
		writeCounter = 0;
		f = new std::ofstream;
		f->exceptions(std::ifstream::failbit | std::ifstream::badbit);
		f->open(filename, std::ios::out | std::ios::trunc);
		out = f;
	}
	~RecentLog() {
		(*out) << cirBuf.print();
		if (fileMode && f->is_open()) {
			f->close();
			delete f;
		}
	}
};

template<typename T>
RecentLog& operator<<(RecentLog& rlog, T const& data) {
	cirBuf.add(data);
	return rlog;
}

#endif
