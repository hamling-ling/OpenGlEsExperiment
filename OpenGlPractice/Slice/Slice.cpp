#include "pch.h"
#include "Slice.h"
#include "Vector3f.h"
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

void Decompose(CVector3f &a, CVector3f &b, CVector3f &c,
			   CVector3f &i0, CVector3f &i1,
			   CTriangle3f* tris)
{
	tris[0].SetValue(a, i0, i1);
	tris[1].SetValue(b, c, i0);
	tris[2].SetValue(c, i1, i0);
}

// test a point p1 is left/right side of line e(between p2 and p3)
//  1: p1 is left side of e
//  0: p1 is on e
// -1: p1 is right side of e
int side( const CVector3f &p1, const CVector3f &p2, const CVector3f &p3 )
{
	const float n  = p1[CVector3f::X] * (p2[CVector3f::Y] - p3[CVector3f::Y])
				   + p2[CVector3f::X] * (p3[CVector3f::Y] - p1[CVector3f::Y])
				   + p3[CVector3f::X] * (p1[CVector3f::Y] - p2[CVector3f::Y]);

	if ( n > 0 )		return  1;
	else if ( n < 0 )	return -1;
	else				return  0;
}

bool SliceTriangle(const CTriangle3f &tri, const CVector3f &s0, const CVector3f &s1, CTriangle3f sliced[3])
{
	CVector3f i0(0.0f,0.0f,0.0f);
	CVector3f i1(0.0f,0.0f,0.0f);

	CVector3f* pA = &(tri[CTriangle3f::A]);
	CVector3f* pB = &(tri[CTriangle3f::B]);
	CVector3f* pC = &(tri[CTriangle3f::C]);

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
