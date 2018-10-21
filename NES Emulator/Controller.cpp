#include "Controller.h"
#include <iostream>

void Controller::IncCycle() {
	cycle = (cycle + 1) % 24; 
	// Taken from NESDoc.pdf, not sure if correct
}

void Controller::SetKey(int keyNum, bool isPressed) {
	keys[keyNum] = isPressed;
}
void Controller::SetJoyKey(int keyNum, bool isPressed) {
	joykeys[keyNum] = isPressed;
}

uint8_t Controller::Read() {
	if (isStrobeOn) {
		// Return state of A when Strobe high
		return (keys[1] & 1) | (joykeys[1] & 1);
	}

	uint8_t val;
	if (cycle < 8) {
		// Return state of keys
		val = (keys[cycle] & 1) | (joykeys[cycle] & 1);
	}
	else {
		// Return 1 after all keys have been exhausted
		val = 1;
	}
	IncCycle();
	return val;
}

void Controller::Write(uint8_t data) {
	bool shouldTurnStrobeOn = ((data & 1) == 1);
	if (isStrobeOn && !shouldTurnStrobeOn) {
		cycle = 0;
	}
	isStrobeOn = shouldTurnStrobeOn;
}