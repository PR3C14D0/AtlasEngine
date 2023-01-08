#include "Module/Time.h"

Time* Time::instance;

Time* Time::GetInstance() {
	if (Time::instance == nullptr)
		Time::instance = new Time();
	return Time::instance;
}

void Time::SetDelta(float deltaTime) {
	this->deltaTime = deltaTime;
}