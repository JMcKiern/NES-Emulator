#pragma once

#include <fstream>
#include <sstream>
#include <cctype>
#include "FunctionalTest.h"
#include "NesTest.h"

bool ToBool(std::string s) {
	for (int i=0; i<s.size(); ++i) {
		s[i] = std::tolower(s[i]);
	}
	if (s == "true") {
		return true;
	}
	else if (s == "false") {
		return false;
	}
	else {
		throw std::runtime_error("Could not parse to bool: " + s);
	}
}

int RunTests(std::string csvTestFile) {
	const char DELIMITER = ',';

	std::ifstream f(csvTestFile);
	std::vector<FunctionalTestRun> ftrs;
	std::vector<NesTestRun> ntrs;

	std::string line;
	while (std::getline(f, line)) {
		std::stringstream ss(line);
		std::vector<std::string> vec;

		std::string token;
		while (std::getline(ss, token, DELIMITER)) {
			vec.push_back(token);
		}

		if (vec.size() == 0) {
			continue; // Empty line
		}
		else if (vec[0][0] == '#') {
			continue; // Comment line
		}
		else if (vec[0] == "FUNCTEST" && vec.size() == 7) {
			FunctionalTestRun ftr;
			ftr.filename = vec[1];
			ftr.memOffset = std::stoi(vec[2]);
			ftr.startPC = std::stoi(vec[3]);
			ftr.successPC = std::stoi(vec[4]);
			ftr.shouldRegIntr = ToBool(vec[5]);
			ftr.shouldPrint = ToBool(vec[6]);
			ftrs.push_back(ftr);
		}
		else if (vec[0] == "NESTEST" && vec.size() == 4) {
			NesTestRun ntr;
			ntr.filename = vec[1];
			ntr.numInstrs = std::stoi(vec[2]);
			ntr.desiredHash = vec[3];
			ntrs.push_back(ntr);
		}
		else {
			throw std::runtime_error("Malformed CSV file.");
		}
	}

	std::vector<std::string> failedFilenames;
	for (auto it = ftrs.begin(); it != ftrs.end(); ++it) {
		bool failed = false;
		try {
			bool testResult = FunctionalTest(it->filename, it->memOffset,
								it->startPC, it->successPC,
								it->shouldRegIntr, it->shouldPrint);
			if (!testResult) {
				failed = true;
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			failed = true;
		}
		if (failed) {
			failedFilenames.push_back(it->filename);
		}

	}
	for (auto it = ntrs.begin(); it != ntrs.end(); ++it) {
		bool failed = false;
		try {
			bool testResult = Test(it->filename, it->numInstrs, it->desiredHash);
			if (!testResult) {
				failed = true;
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			failed = true;
		}
		if (failed) {
			failedFilenames.push_back(it->filename);
		}
	}
	std::cout << "All tests completed\n";
	if (failedFilenames.size() > 0) {
		std::cerr << "Failed: \n";
		for (auto it = failedFilenames.begin();
		     it != failedFilenames.end();
		     ++it) {
			std::cerr << (*it) << '\n';
		}
		return 1;
	}
	else {
		std::cout << "Passed all tests!\n";
		return 0;
	}
}
