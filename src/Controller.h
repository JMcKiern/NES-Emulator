#pragma once

#include <cstdint>

class Controller {
private:
	bool isStrobeOn = true;
	int cycle = 0;

	// In order: A, B, Select, Start, Up, Down, Left, Right
	bool keys[8];
	bool joykeys[8];

	void IncCycle();

public:
	void SetKey(int keyNum, bool isPressed);
	void SetJoyKey(int keyNum, bool isPressed);
	uint8_t Read();
	void Write(uint8_t data);

	Controller();
};

