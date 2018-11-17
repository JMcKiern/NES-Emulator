#include "PeripheralConnection.h"

DigitalState PeripheralConnection::GetState() {
	return lineState;
}
void PeripheralConnection::SetState(DigitalState _lineState) {
	lineState = _lineState;
}
PeripheralConnection::PeripheralConnection() {
	lineState = HIGH;
}