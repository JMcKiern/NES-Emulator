#pragma once

enum DigitalState {
	LOW,
	HIGH
};

class PeripheralConnection {
private:
	DigitalState lineState;
public:
	DigitalState GetState();
	void SetState(DigitalState _isOn);
	PeripheralConnection();
};
