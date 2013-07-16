#pragma once

#include "Triangle.h"
#include "Triangle3v.h"
#include "Plane.h"
#include "Line.h"
#include "Matrix4x4f.h"

#define EPSILON		(1e-5)
#define FEQ(x,y)	(abs((x)-(y))<EPSILON)

typedef struct SLICERESULT {
	int RightTriangleCount;
	int LeftTriangleCount;
	CTriangle RightTriangles[2];
	CTriangle LeftTriangles[2];
} SliceResult;

typedef struct SLICERESULT3v {
	int NormalSideCount;
	int AntinormalSideCount;
	CTriangle3v NormalSides[2];
	CTriangle3v AntinormalSides[2];
} SliceResult3v;

bool TryGetIntersection3v(const CVertex& r0, const CVertex& r1, const CVector3f& n, const CVector3f& p, CVertex& i0, bool &isEndPoint);

void Decompose(CVector3f &a, CVector3f &b, CVector3f &c,
			   CVector3f &i0, CVector3f &i1,
			   CTriangle* tris);

bool SliceTriangle3v(const CTriangle3v &tri, const CPlane &plane, SliceResult3v& sliceResult);
