#include "stdafx.h"
#include "Slice.h"
#include "Vector3f.h"
#include "Vertex.h"
#include "float.h"
#include <algorithm>

using namespace std;


// ray(point r0 to point r1)
// plane(specified by normal and any point on the plane p)
// see http://www.thepolygoners.com/tutorials/lineplane/lineplane.html
bool TryGetIntersection3v(const CVertex& r0, const CVertex& r1, const CVector3f& n, const CVector3f& p, CVertex& i0, bool &isEndPoint)
{
	isEndPoint = false;

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
	isEndPoint = !(0 < t && t < 1.0);
	if(isEndPoint) {
		if(t == 0.0)
			i0v = r0v;
		else // t==1.0
			i0v = r1v;
	}
	else {
		i0v = r0v + t*(r1v - r0v);
	}

	i0.SetValue(r0.GetNormal(), i0v);

	return true;
}


void Decompose3v(CVertex &a, CVertex &b, CVertex &c,
				 CVertex &i0, CVertex &i1,
				 CTriangle3v* tris)
{
	tris[0].SetValue(a, i0, i1);
	tris[1].SetValue(b, c, i0);
	tris[2].SetValue(c, i1, i0);
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
	int decompedLen = 1;
	decomped[0] = tri;

	bool result = false;
	bool isEndPoint0 = false;
	bool isEndPoint1 = false;;
	if(TryGetIntersection3v(*pA,*pB,*pn,*pp,i0, isEndPoint0)) {
		if(TryGetIntersection3v(*pB,*pC,*pn,*pp,i1, isEndPoint1)) {
			if(!isEndPoint0 && !isEndPoint1) {
				Decompose3v(*pB,*pC,*pA,i1,i0,decomped);
				decompedLen = 3;
				result = true;
			}
		}
		else if(TryGetIntersection3v(*pC,*pA,*pn,*pp,i1, isEndPoint0)) {
			if(!isEndPoint0 && !isEndPoint1) {
				Decompose3v(*pA,*pB,*pC,i0,i1,decomped);
				decompedLen = 3;
				result = true;
			}
		}
	}
	else if(TryGetIntersection3v(*pB,*pC,*pn,*pp,i0, isEndPoint0)) {
		if(TryGetIntersection3v(*pC,*pA,*pn,*pp,i1, isEndPoint1)) {
			if(!isEndPoint0 && !isEndPoint1) {
				Decompose3v(*pC,*pA,*pB,i1,i0,decomped);
				decompedLen = 3;
				result = true;
			}
		}
	}

	// determine a side and set result
	memset(&sliceResult, 0, sizeof(sliceResult));
	for(int i = 0; i < decompedLen; i++) {
		SortSides(decomped[i], *pn, *pp, sliceResult);
	}

	return result;
}