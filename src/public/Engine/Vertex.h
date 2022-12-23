#pragma once

typedef float RGBA[4];
typedef float tCoord[2];

struct vertex {
	float x, y, z;
	tCoord texCoords;
};