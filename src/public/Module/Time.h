#pragma once
#include <iostream>

class Time {
	friend class Window;
private:
	static Time* instance;
	void SetDelta(float deltaTime);
public:
	Time() = default;
	float deltaTime;
	static Time* GetInstance();
};