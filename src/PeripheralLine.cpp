#include "PeripheralLine.h"

void PeripheralLine::AddConnection(PeripheralConnection*
                                       _peripheralConnection) {
	connections.push_back(_peripheralConnection);
}
void PeripheralLine::RemoveConnection(PeripheralConnection*
                                          _peripheralConnection) {
	connections.erase(
		std::remove(connections.begin(),
		            connections.end(),
		            _peripheralConnection),
		connections.end());
}
DigitalState PeripheralLine::GetState() {
	DigitalState lineState = HIGH;
	for (std::vector<PeripheralConnection*>::iterator it = connections.begin();
	     it != connections.end();
	     ++it) {
		if ((*it)->GetState() == LOW) {
			lineState = LOW;
			break;
		}
	}
	return lineState;
}