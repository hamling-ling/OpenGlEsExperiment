#include "stdafx.h"
#include "Slice.h"
#include "Vector3f.h"
#include "Vertex.h"
#include "float.h"
#include <algorithm>

using namespace std;


bool OnThePlane(const CVector3f& r0, const CVector3f& r1, const CVector3f& n, const CVector3f& p)
{
	CVector3f r0_p = p - r0;
	CVector3f r1_p = p - r1;
	CVector3f r0_p_cross_r1_p = r0_p.Cross(r1_p);

	float n_cross_len = n.Length() * r0_p_cross_r1_p.Length();
	if(n_cross_len == 0.0)
		return false; // bug

	float cosT = n.Dot(r0_p_cross_r1_p) / n_cross_len;
	if(fabs(cosT) == 1.0)
		return true;
	else
		return false;
}


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
		if(OnThePlane(r0v, r1v, n, p))
			return OnParallel;
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


void Decompose3(const CVertex &a, const CVertex &b, const CVertex &c,
				const CVertex &i0, const CVertex &i1,
				CTriangle3v* tris)
{
	tris[0].SetValue(a, i0, i1);
	tris[1].SetValue(b, c, i0);
	tris[2].SetValue(c, i1, i0);
}


void Decompose2(const CVertex &a, const CVertex &b, const CVertex &c,
				const CVertex &i1, CTriangle3v* tris)
{
	tris[0].SetValue(a, b, i1);
	tris[1].SetValue(a, i1, b);
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
	memset(&sliceResult, 0, sizeof(sliceResult));

	bool result = false;
	for(int i = 0; i < 3; i++) {
		CVertex* pA = &(tri[(i+0)%3]);
		CVertex* pB = &(tri[(i+1)%3]);
		CVertex* pC = &(tri[(i+2)%3]);

		IntersectionType firstResult = TryGetIntersection3v(*pA,*pB,*pn,*pp,i0);
		if(BetweenEnds == firstResult) {
			IntersectionType secondResult = TryGetIntersection3v(*pB,*pC,*pn,*pp,i1);
			if(BetweenEnds == secondResult) {
				Decompose3( *pB, *pC, *pA, i1, i0, decomped);
				decompedLen = 3;
				result = true;
				// save intersection
				sliceResult.Intersections[0] = i0;
				sliceResult.Intersections[1] = i1;
				sliceResult.InterSectionCount = 2;
				break;
			}
		}
		else if(OnFirstEnd == firstResult) {
			IntersectionType secondResult = TryGetIntersection3v(*pB,*pC,*pn,*pp,i1);
			if(BetweenEnds == secondResult) {
				Decompose2( *pA, *pB, *pC, i1, decomped);
				decompedLen = 2;
				result = true;
				// save intersection
				sliceResult.Intersections[0] = *pA;
				sliceResult.Intersections[1] = i1;
				sliceResult.InterSectionCount = 2;
				break;
			}
		}
		else if(OnParallel == firstResult) {
			// save intersection
			sliceResult.Intersections[0] = *pA;
			sliceResult.Intersections[1] = *pB;
			sliceResult.InterSectionCount = 2;
			break;
		}
	}

	// determine a side and set result
	for(int i = 0; i < decompedLen; i++) {
		SortSides(decomped[i], *pn, *pp, sliceResult);
	}

	return result;
}


bool GetClosedIntersections(const list<CLine>& lines, vector<CVector3f>& closedIntersections)
{
	closedIntersections.clear();

	if(lines.size() < 3) {
		return false;
	}

	list<CLine> intersections(lines.begin(), lines.end());
	list<CLine>::iterator it = intersections.begin();
	closedIntersections.push_back((*it)[CLine::A]);
	closedIntersections.push_back((*it)[CLine::B]);
	intersections.pop_front();

	it = intersections.begin();
	CVector3f connection, nonconnection;

	while(intersections.size() > 0) {
		
		list<CLine>::iterator inner_it = it;
		bool found = false;
		while(inner_it != intersections.end()) {
			CVector3f connection, nonconnection;
			if(IsConnecting(*(closedIntersections.rbegin()), *inner_it, connection, nonconnection)) {
				closedIntersections.push_back(connection);
				intersections.erase(inner_it);
				found = true;
				break;
			}
			inner_it++;
		}
		if(!found)
			break;
	}

	// if it is closed
	vector<CVector3f>::const_reverse_iterator last = closedIntersections.rbegin();
	if( closedIntersections.size() > 2 &&
		closedIntersections.begin()->NearlyEquals(*last)) {
			// yes, it's closed
			closedIntersections.pop_back();
	}
	else {
		closedIntersections.clear();
	}

	return !closedIntersections.empty();
}


bool IsConnecting(const CVector3f& p, const CLine& line, CVector3f& connection, CVector3f nonconnection)
{
	if(line[CLine::A].NearlyEquals(p)) {
		connection = line[CLine::B];
		nonconnection = line[CLine::A];
		return true;
	}
	else if(line[CLine::B].NearlyEquals(p)) {
		connection = line[CLine::A];
		nonconnection = line[CLine::B];
		return true;
	}
	return false;
}


CVector3f GetNormal(vector<CVector3f>& closedIntersections)
{
	CVector3f normal;
	for(int i = 0; i < closedIntersections.size()-1; i++) {
		int size = closedIntersections.size();
		int idxa = (i+0)%size; // a
		int idxb = (i+1)%size; // b
		int idxc = (i+2)%size; // c
		CVector3f ba = closedIntersections[idxa] - closedIntersections[idxb];
		CVector3f bc = closedIntersections[idxc] - closedIntersections[idxb];
		normal += ba.Cross(bc);
	}

	normal /= closedIntersections.size();

	return normal;
}


bool CanSnip(const int idxa, const int idxb, const int idxc,
			 const vector<CVector3f>& closedIntersections,
			 const CVector3f& normal)
{
	CVector3f ba = closedIntersections[idxa] - closedIntersections[idxb];
	CVector3f bc = closedIntersections[idxc] - closedIntersections[idxb];
	CVector3f baxbc=ba.Cross(bc);
	float denom = normal.Length() * baxbc.Length();
	if(FEQ(denom, 0.0f))
		return false;

	float cosT = baxbc.Dot(normal)/denom;
	if(!FEQ(cosT, 1.0f))
		return false;

	int size = closedIntersections.size();
	for(int idx = 0; idx < size; idx++) {
		if(idx == idxa || idx == idxb || idx == idxc) {
			continue;
		}
		if(IsInside(idx, idxa, idxb, idxc, closedIntersections)) {
			return false;
		}
	}

	return true;
}


bool IsInside(const int idxp, const int idxa, const int idxb, const int idxc, const vector<CVector3f>& closedIntersections)
{
	CVector3f ab = closedIntersections[idxb] - closedIntersections[idxa];
	CVector3f bc = closedIntersections[idxc] - closedIntersections[idxb];
	CVector3f ca = closedIntersections[idxa] - closedIntersections[idxc];
	CVector3f ap = closedIntersections[idxp] - closedIntersections[idxa];
	CVector3f bp = closedIntersections[idxp] - closedIntersections[idxb];
	CVector3f cp = closedIntersections[idxp] - closedIntersections[idxc];

	CVector3f e1 = ab.Cross(ap);
	CVector3f e2 = bc.Cross(bp);
	CVector3f e3 = ca.Cross(cp);

	float cosT1 = e1.Dot(e2);
	float cosT2 = e2.Dot(e3);
	float cosT3 = e3.Dot(e1);

	if(ISPOSITIVE(cosT1) && ISPOSITIVE(cosT2) && ISPOSITIVE(cosT3)) {
		return true;
	}

	return false;
}


void Snip(const int idxa, const int idxb, const int idxc,
		  vector<CVector3f>& closedIntersections,
		  list<CTriangle3v>& triangles)
{
	CVector3f normal;
	CVertex va(normal, closedIntersections[idxa]);
	CVertex vb(normal, closedIntersections[idxb]);
	CVertex vc(normal, closedIntersections[idxc]);

	CTriangle3v tri(va, vb, vc);
	triangles.push_back(tri);

	closedIntersections.erase(closedIntersections.begin() + idxb);
}

