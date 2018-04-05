#pragma once

#include <cstdint>

class Controller {
private:
	bool isStrobeOn; // TODO: What is initial state?
	int cycle = 0;

	// In order: A, B, Select, Start, Up, Down, Left, Right
	bool keys[8];
	uint8_t key;

	void IncCycle();

public:
	void SetKey(int keyNum, bool isPressed);
	uint8_t Read();
	void Write(uint8_t data);
};

