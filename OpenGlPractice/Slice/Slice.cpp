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
IntersectionType TryGetIntersection3v(const CVertex& r0, const CVertex& r1, const CVector3f& n, const CVector3f& p, CVertex& i0)
{
	IntersectionType result = NoIntersection;

	const CVector3f r0v(r0.GetPoint());
	CVector3f r1v = r1.GetPoint();
	float NdotP2P0 = n.Dot(p - r0.GetPoint());
	CVector3f r0_r1 = r1.GetPoint() - r0.GetPoint();
	float NdotV = n.Dot(r0_r1);

	if(NdotV == 0.0) {
		// N perp V
		CVector3f r0_p = p - r0v;
		CVector3f r1_p = p - r1v;
		CVector3f cross = r0_p.Cross(r1_p);
		float n_cross = n.Length() * cross.Length();
		if(n_cross == 0.0)
			return OffParallel; // bug

		float cosT = n.Dot(cross) / n_cross;
		if(fabs(cosT) == 1.0)
			return OnParallel;
		else
			return OffParallel;
	}

	float t = NdotP2P0/NdotV;
	if(t < 0.0)
		return BeforeFirstEnd;
	if( 1.0 < t)
		return BeyondSecondEnd;

	CVector3f i0v; // intersection
	if(0 < t && t < 1.0) {
		i0v = r0v + t*(r1v - r0v);
		result = BetweenEnds;
	}
	else if(t == 0.0) {
		i0v = r0v;
		result = OnFirstEnd;
	}
	else {// t==1.0
		i0v = r1v;
		result = OnSecondEnd;
	}

	i0.SetValue(r0.GetNormal(), i0v);

	return result;
}


void Decompose3v(const CVertex &a, const CVertex &b, const CVertex &c,
				 const CVertex &i0, const CVertex &i1,
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



	CVector3f* pn = &(plane[CPlane::N]);
	CVector3f* pp = &(plane[CPlane::P]);

	CTriangle3v decomped[3];
	int decompedLen = 1;
	decomped[0] = tri;

	bool result = false;
	for(int i = 0; i < 3; i++) {
		CVertex* pA = &(tri[(i+0)%3]);
		CVertex* pB = &(tri[(i+1)%3]);
		CVertex* pC = &(tri[(i+2)%3]);

		IntersectionType firstResult = TryGetIntersection3v(*pA,*pB,*pn,*pp,i0);
		if(firstResult == BetweenEnds) {
			IntersectionType secondResult = TryGetIntersection3v(*pB,*pC,*pn,*pp,i1);
			if(secondResult == BetweenEnds) {
				Decompose3v(*pB,*pC,*pA,i1,i0,decomped);
				decompedLen = 3;
				result = true;
				break;
			}
			else if(TryGetIntersection3v(*pC,*pA,*pn,*pp,i1)) {
				Decompose3v(*pA,*pB,*pC,i0,i1,decomped);
				decompedLen = 3;
				result = true;
				break;
			}
		}
		else if(firstResult == OnParallel) {
			break;
		}
	}

	// determine a side and set result
	memset(&sliceResult, 0, sizeof(sliceResult));
	for(int i = 0; i < decompedLen; i++) {
		SortSides(decomped[i], *pn, *pp, sliceResult);
	}

	return result;
}