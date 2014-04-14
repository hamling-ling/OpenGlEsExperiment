#pragma once

#ifndef PI
#define PI  3.14159265358979323846f
#endif
#define EPSILON		(1e-5f)
#define EPSIOLONLOW	(1e-3f)
#define FEQ(x,y)	(fabs((x)-(y))<EPSILON)
#define FEQLOW(x,y)	(fabs((x)-(y))<EPSIOLONLOW)
#define ISPOSITIVE(x)	((x) > 0.0f)

#include "modeltool.h"

using namespace osakanaengine;

static inline bool NearlyEquals(MODELVEC3D a, MODELVEC3D b)
{
	return FEQ(a.x, b.x) && FEQ(a.y, b.y) && FEQ(a.z, b.z);
}

static inline bool NearlyEquals(MODELLINE a, MODELLINE b)
{
	return NearlyEquals(a.a, a.b);
}

static inline MODELVEC3D GetCenter(const MODELTRIANGLE& tri)
{
	MODELVEC3D sum = tri.a.point;
	sum = AddVec(sum, tri.b.point);
	sum = AddVec(sum, tri.c.point);
	
	return ScaleVec(static_cast<MODELFLOAT>(1.0/3.0), sum);
}

static inline bool IsSameDirection(const MODELVEC3D& a, const MODELVEC3D& b)
{
	float denom = Magnitude(a) * Magnitude(b);
	if(FEQ(denom, 0.0f))
		return false;
	
	float cosT = DotProduct(a,b)/denom;
	if(FEQ(cosT, 1.0f))
		return true;
	return false;
}

static inline void SetNormal(MODELTRIANGLE& tri, const MODELVEC3D& n)
{
	tri.v[0].normal = n;
	tri.v[1].normal = n;
	tri.v[2].normal = n;
}
