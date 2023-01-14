#include "Module/Util.h"

std::string Util::XYZToString(float x, float y, float z, bool addCoordName) {
	std::string stringified;
	
	std::string sX = std::to_string(x);
	std::string sY = std::to_string(y);
	std::string sZ = std::to_string(z);

	if (addCoordName)
		stringified = "[X] " + sX + " [Y] " + sY + " [Z] " + sZ;
	else
		stringified = sX + " " + sY + " " + sZ;

	return stringified;
}