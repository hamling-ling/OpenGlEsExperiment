#pragma once

#include "Triangle3f.h"
#include "Matrix4x4f.h"

#define EPSILON		(1e-5)
#define FEQ(x,y)	(abs((x)-(y))<EPSILON)

bool TryGetIntersection(CVector3f p0, CVector3f p2, CVector3f p1, CVector3f p3,CVector3f &i);

void Decompose(CVector3f &a, CVector3f &b, CVector3f &c,
			   CVector3f &i0, CVector3f &i1,
			   CTriangle3f* tris);

int side( const CVector3f &p1, const CVector3f &p2, const CVector3f &p3 );

bool SliceTriangle(const CTriangle3f &tri, const CVector3f &s0, const CVector3f &s1, CTriangle3f sliced[3]);
