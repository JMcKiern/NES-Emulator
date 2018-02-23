#pragma once

class PeripheralConnection {
private:
	bool isOn;
public:
	bool GetState();
	void SetState(bool _isOn);
};
