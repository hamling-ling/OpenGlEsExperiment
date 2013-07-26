#pragma once

#include <list>
#include <vector>
#include "Triangle.h"
#include "Triangle3v.h"
#include "Plane.h"
#include "Line.h"
#include "Matrix4x4f.h"

using namespace std;


typedef struct SLICERESULT3v {
	int NormalSideCount;
	int AntinormalSideCount;
	CTriangle3v NormalSides[2];
	CTriangle3v AntinormalSides[2];
	CVertex Intersections[2];
	int InterSectionCount;
} SliceResult3v;

void Chop(const CPlane& plane, const float* normalsAndVertices, const int len,
		  float bufN[64][8], float bufA[64][8],
		  int& bufNCount, int& bufACount);
