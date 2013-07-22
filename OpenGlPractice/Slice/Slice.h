#pragma once

#include <list>
#include <vector>
#include "Triangle.h"
#include "Triangle3v.h"
#include "Plane.h"
#include "Line.h"
#include "Matrix4x4f.h"

using namespace std;

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
	CVertex Intersections[2];
	int InterSectionCount;
} SliceResult3v;

typedef enum INTERSECTIONTYPE {
	NoIntersection,
	BetweenEnds,
	OnFirstEnd,
	OnSecondEnd,
	BeforeFirstEnd,
	BeyondSecondEnd,
	OnParallel,
	OffParallel,
} IntersectionType;

IntersectionType TryGetIntersection3v(const CVertex& r0, const CVertex& r1, const CVector3f& n, const CVector3f& p, CVertex& i0);

void Decompose3v(const CVertex &a, const CVertex &b, const CVertex &c,
				 const CVertex &i0, const CVertex &i1,
				 CTriangle3v* tris);

bool SliceTriangle3v(const CTriangle3v &tri, const CPlane &plane, SliceResult3v& sliceResult);

bool GetClosedIntersections(const list<CLine>& intersections, vector<CVector3f>& closedIntersections);

bool IsConnecting(const CVector3f& p, const CLine& line, CVector3f& connection, CVector3f nonconnection);

bool CanSnip(const int idxa, const int idxb, const int idxc, const vector<CVector3f>& closedIntersections);

bool IsInside(const int idxp, const int idxa, const int idxb, const int idxc,
		  const vector<CVector3f>& closedIntersections);

void Snip(const int idxa, const int idxb, const int idxc,
		  vector<CVector3f>& closedIntersections,
		  list<CTriangle3v>& triangles);
