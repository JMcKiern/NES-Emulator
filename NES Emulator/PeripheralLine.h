#pragma once

#include <vector>
#include <algorithm>
#include "PeripheralConnection.h"

class PeripheralLine {
private:
	std::vector<PeripheralConnection*> connections;
	int nextIndex = 0;
public:
	void AddConnection(PeripheralConnection* _peripheralConnection);
	void RemoveConnection(PeripheralConnection* _peripheralConnection);
	DigitalState GetState();
};