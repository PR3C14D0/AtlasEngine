#pragma once

typedef float RGBA[4];
typedef float position[3];
typedef float tCoord[2];
typedef float normals[3];

struct vertex {
	position xyz;
	tCoord texCoords;
	normals normalVertex;
};