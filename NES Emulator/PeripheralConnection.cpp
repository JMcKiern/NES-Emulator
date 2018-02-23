#include "PeripheralConnection.h"

bool PeripheralConnection::GetState() {
	return isOn;
}
void PeripheralConnection::SetState(bool _isOn) {
	isOn = _isOn;
}