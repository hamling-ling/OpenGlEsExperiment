#include "stdafx.h"
#include "Slice.h"
#include "Vector3f.h"
#include "Vertex.h"
#include "float.h"
#include <algorithm>

using namespace std;

bool TryGetIntersection(CVector3f p0, CVector3f p2, CVector3f p1, CVector3f p3,CVector3f &i)
{
	p0[CVector3f::Z] = 0.0f; p1[CVector3f::Z] = 0.0f; p2[CVector3f::Z] = 0.0f; p3[CVector3f::Z] = 0.0f; 

	//vector a1：p1->p3
	CVector3f a1 = p3 - p1;
	//vector a2：p0->p2
	CVector3f a2 = p2 - p0;
	//vector b1：p2->p1
	CVector3f b1 = p1 - p2;
	//vector b2：p1->p0
	CVector3f b2 = p0 - p1;

	//area S1 = (a1 x b2)/2
	float s1 = a1.Cross(b2)[CVector3f::Z]/2.0f;
	//area S2 = (a1 x b1)/2
	float s2 = a1.Cross(b1)[CVector3f::Z]/2.0f;

	//c1.X = p0.X + (p2.X - p0.X) * s1 / (s1 + s2)
	float x = p0[CVector3f::X] + (p2[CVector3f::X]-p0[CVector3f::X])*s1/(s1+s2);
	//c1.Y = p0.Y + (p2.Y - p0.Y) * s1 / (s1 + s2)
	float y = p0[CVector3f::Y] + (p2[CVector3f::Y]-p0[CVector3f::Y])*s1/(s1+s2);
	i.SetValue(x, y, 0.0f);

	float min_x = min(p0[CVector3f::X], p2[CVector3f::X]);
	float max_x = max(p0[CVector3f::X], p2[CVector3f::X]);
	float min_y = min(p0[CVector3f::Y], p2[CVector3f::Y]);
	float max_y = max(p0[CVector3f::Y], p2[CVector3f::Y]);
	return (min_x <= i[CVector3f::X]) && (i[CVector3f::X] <= max_x) && (min_y <= i[CVector3f::Y]) && (i[CVector3f::Y] <= max_y) ;
}

// ray(point r0 to point r1)
// plane(specified by normal and any point on the plane p)
// see http://www.thepolygoners.com/tutorials/lineplane/lineplane.html
bool TryGetIntersection3v(const CVertex& r0, const CVertex& r1, const CVector3f& n, const CVector3f& p, CVertex& i0)
{
	const CVector3f r0v(r0.GetPoint());
	CVector3f r1v = r1.GetPoint();
	float NdotP2P0 = n.Dot(p - r0.GetPoint());
	float NdotV = n.Dot(r1.GetPoint() - r0.GetPoint());
	if(NdotV == 0.0)
		return false;

	float t = NdotP2P0/NdotV;
	if(t < 0.0 || 1.0 < t)
		return false;

	CVector3f i0v; // intersection
	if(0 < t && t < 1.0)
		i0v = r0v + t*(r1v - r0v);
	else if(t == 0.0)
		i0v = r0v;
	else // t==1.0
		i0v = r1v;

	i0.SetValue(r0.GetNormal(), i0v);

	return true;
}

void Decompose(CVector3f &a, CVector3f &b, CVector3f &c,
			   CVector3f &i0, CVector3f &i1,
			   CTriangle* tris)
{
	tris[0].SetValue(a, i0, i1);
	tris[1].SetValue(b, c, i0);
	tris[2].SetValue(c, i1, i0);
}

void Decompose3v(CVertex &a, CVertex &b, CVertex &c,
				 CVertex &i0, CVertex &i1,
				 CTriangle3v* tris)
{
	tris[0].SetValue(a, i0, i1);
	tris[1].SetValue(b, c, i0);
	tris[2].SetValue(c, i1, i0);
}

// test a point p1 is left/right side of line l(between p2 and p3)
//  1: p1 is left side of l
//  0: p1 is on l
// -1: p1 is right side of l
int side( const CVector3f &p1, const CVector3f &p2, const CVector3f &p3 )
{
	const float n  = p1[CVector3f::X] * (p2[CVector3f::Y] - p3[CVector3f::Y])
				   + p2[CVector3f::X] * (p3[CVector3f::Y] - p1[CVector3f::Y])
				   + p3[CVector3f::X] * (p1[CVector3f::Y] - p2[CVector3f::Y]);

	if ( n > 0 )		return  1;
	else if ( n < 0 )	return -1;
	else				return  0;
}

bool SliceTriangle(const CTriangle &tri, const CVector3f &s0, const CVector3f &s1, CTriangle sliced[3])
{
	CVector3f i0(0.0f,0.0f,0.0f);
	CVector3f i1(0.0f,0.0f,0.0f);

	CVector3f* pA = &(tri[CTriangle::A]);
	CVector3f* pB = &(tri[CTriangle::B]);
	CVector3f* pC = &(tri[CTriangle::C]);

	bool result = true;
	if(TryGetIntersection(*pA,*pB,s0,s1,i0))
	{
		if(TryGetIntersection(*pB,*pC,s0,s1,i1))
		{
			Decompose(*pB,*pC,*pA,i1,i0,sliced);
		}
		else if(TryGetIntersection(*pC,*pA,s0,s1,i1))
		{
			Decompose(*pA,*pB,*pC,i0,i1,sliced);
		}
		else
		{
			result = false;
		}
	}
	else if(TryGetIntersection(*pB,*pC,s0,s1,i0))
	{
		if(TryGetIntersection(*pC,*pA,s0,s1,i1))
		{
			Decompose(*pC,*pA,*pB,i1,i0,sliced);
		}
		else
		{
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

bool SliceTriangle(const CTriangle &tri, const CLine &line, SliceResult& sliceResult)
{
	CVector3f i0(0.0f,0.0f,0.0f);
	CVector3f i1(0.0f,0.0f,0.0f);

	CVector3f* pA = &(tri[CTriangle::A]);
	CVector3f* pB = &(tri[CTriangle::B]);
	CVector3f* pC = &(tri[CTriangle::C]);

	CVector3f* pS0 = &(line[CTriangle::A]);
	CVector3f* pS1 = &(line[CTriangle::B]);

	CTriangle decomped[3];
	int decompedLen = 3;

	bool result = true;
	if(TryGetIntersection(*pA,*pB,*pS0,*pS1,i0))
	{
		if(TryGetIntersection(*pB,*pC,*pS0,*pS1,i1))
		{
			Decompose(*pB,*pC,*pA,i1,i0,decomped);
		}
		else if(TryGetIntersection(*pC,*pA,*pS0,*pS1,i1))
		{
			Decompose(*pA,*pB,*pC,i0,i1,decomped);
		}
		else
		{
			decomped[0] = tri;
			decompedLen = 1;
			result = false;
		}
	}
	else if(TryGetIntersection(*pB,*pC,*pS0,*pS1,i0))
	{
		if(TryGetIntersection(*pC,*pA,*pS0,*pS1,i1))
		{
			Decompose(*pC,*pA,*pB,i1,i0,decomped);
		}
		else
		{
			decomped[0] = tri;
			decompedLen = 1;
			result = false;
		}
	}
	else
	{
		decomped[0] = tri;
		decompedLen = 1;
		result = false;
	}

	memset(&sliceResult, 0, sizeof(sliceResult));

	for(int i = 0; i < decompedLen; i++) {
		CVector3f center = decomped[i].GetCenter();
		if(side(center, line[CLine::A], line[CLine::B]) >= 0) {
			// left
			sliceResult.LeftTriangles[sliceResult.LeftTriangleCount] = decomped[i];
			sliceResult.LeftTriangleCount++;
		}
		else {
			// right
			sliceResult.RightTriangles[sliceResult.RightTriangleCount] = decomped[i];
			sliceResult.RightTriangleCount++;
		}
	}

	return result;
}

/**
	test which side of a plane a given triangle is belonged to.
	@param tri triangle
	@param n normal vector of a plane
	@param p any point on a plane
	@param sliceResult result
 */
void SortSides(const CTriangle3v &tri, const CVector3f& n, const CVector3f& p, SliceResult3v& sliceResult)
{
	CVector3f a = tri.GetCenter();
	CVector3f p_a = a - p;

	float denominator = p_a.Length() * n.Length();
	if(denominator != 0.0) {
		float numerator = p_a.Dot(n);
		float cosT = numerator / denominator;
		if(cosT >= 0.0) {
			// normal vector side
			sliceResult.NormalSides[sliceResult.NormalSideCount] = tri;
			sliceResult.NormalSideCount++;
		}
		else {
			// anti-normal vector side
			sliceResult.AntinormalSides[sliceResult.AntinormalSideCount] = tri;
			sliceResult.AntinormalSideCount++;
		}
	}
	else {
		// a is on the plane
		// treate as normal side
		sliceResult.NormalSides[sliceResult.NormalSideCount] = tri;
		sliceResult.NormalSideCount++;
	}
}

bool SliceTriangle3v(const CTriangle3v &tri, const CPlane &plane, SliceResult3v& sliceResult)
{
	CVertex i0,i1;

	CVertex* pA = &(tri[CTriangle3v::A]);
	CVertex* pB = &(tri[CTriangle3v::B]);
	CVertex* pC = &(tri[CTriangle3v::C]);

	CVector3f* pn = &(plane[CPlane::N]);
	CVector3f* pp = &(plane[CPlane::P]);

	CTriangle3v decomped[3];
	int decompedLen = 3;

	bool result = true;
	if(TryGetIntersection3v(*pA,*pB,*pn,*pp,i0))
	{
		if(TryGetIntersection3v(*pB,*pC,*pn,*pp,i1))
		{
			Decompose3v(*pB,*pC,*pA,i1,i0,decomped);
		}
		else if(TryGetIntersection3v(*pC,*pA,*pn,*pp,i1))
		{
			Decompose3v(*pA,*pB,*pC,i0,i1,decomped);
		}
		else
		{
			decomped[0] = tri;
			decompedLen = 1;
			result = false;
		}
	}
	else if(TryGetIntersection3v(*pB,*pC,*pn,*pp,i0))
	{
		if(TryGetIntersection3v(*pC,*pA,*pn,*pp,i1))
		{
			Decompose3v(*pC,*pA,*pB,i1,i0,decomped);
		}
		else
		{
			decomped[0] = tri;
			decompedLen = 1;
			result = false;
		}
	}
	else
	{
		decomped[0] = tri;
		decompedLen = 1;
		result = false;
	}

	// determine a side and set result
	memset(&sliceResult, 0, sizeof(sliceResult));
	for(int i = 0; i < decompedLen; i++) {
		SortSides(decomped[i], *pn, *pp, sliceResult);
	}

	return result;
}