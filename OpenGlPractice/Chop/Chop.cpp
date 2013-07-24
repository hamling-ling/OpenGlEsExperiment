#include "stdafx.h"

#include <algorithm>
#include "float.h"

#include "Chop.h"
#include "Vector3f.h"
#include "Vertex.h"

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


bool ChopTriangle3v(const CTriangle3v &tri, const CPlane &plane, SliceResult3v& sliceResult)
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


bool GetContour(const list<CLine>& lines, vector<CVector3f>& contour)
{
	contour.clear();

	if(lines.size() < 3) {
		return false;
	}

	list<CLine> intersections(lines.begin(), lines.end());
	list<CLine>::iterator it = intersections.begin();
	contour.push_back((*it)[CLine::A]);
	contour.push_back((*it)[CLine::B]);
	intersections.pop_front();

	it = intersections.begin();
	CVector3f connection, nonconnection;

	while(intersections.size() > 0) {
		
		list<CLine>::iterator inner_it = it;
		bool found = false;
		while(inner_it != intersections.end()) {
			CVector3f connection, nonconnection;
			if(IsConnecting(*(contour.rbegin()), *inner_it, connection, nonconnection)) {
				contour.push_back(connection);
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
	vector<CVector3f>::const_reverse_iterator last = contour.rbegin();
	if( contour.size() > 2 &&
		contour.begin()->NearlyEquals(*last)) {
			// yes, it's closed
			contour.pop_back();
	}
	else {
		contour.clear();
	}

	return !contour.empty();
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
		CVector3f B = closedIntersections[idxa] - closedIntersections[idxb];
		CVector3f A = closedIntersections[idxc] - closedIntersections[idxb];
		normal += A.Cross(B);
	}

	normal /= closedIntersections.size();

	return normal;
}


bool CanSnip(const int idxa, const int idxb, const int idxc,
			 const vector<CVector3f>& closedIntersections,
			 const CVector3f& normal)
{
	CVector3f B = closedIntersections[idxa] - closedIntersections[idxb];
	CVector3f A = closedIntersections[idxc] - closedIntersections[idxb];
	CVector3f AxB=A.Cross(B);
	if(!AxB.IsSameDirection(normal))
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


bool IsInside(const int idxp, const int idxa, const int idxb, const int idxc, const vector<CVector3f>& contour)
{
	CVector3f ab = contour[idxb] - contour[idxa];
	CVector3f bc = contour[idxc] - contour[idxb];
	CVector3f ca = contour[idxa] - contour[idxc];
	CVector3f ap = contour[idxp] - contour[idxa];
	CVector3f bp = contour[idxp] - contour[idxb];
	CVector3f cp = contour[idxp] - contour[idxc];

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
		  vector<CVector3f>& contour,
		  list<CTriangle3v>& triangles)
{
	CVector3f normal;
	CVertex va(normal, contour[idxa]);
	CVertex vb(normal, contour[idxb]);
	CVertex vc(normal, contour[idxc]);

	CTriangle3v tri(va, vb, vc);
	triangles.push_back(tri);

	contour.erase(contour.begin() + idxb);
}


void Chop(const CPlane& plane, const float* normalsAndVertices, const int len,
		  float bufN[64][6], float bufA[64][6],
		  int& bufNCount, int& bufACount)
{
	bufNCount = 0;
	bufACount = 0;

	SliceResult3v sliceResult;
	list<CLine> intersections;
	for(int vertCount = 0; vertCount < len; vertCount+=3)
	{
		CVertex a(normalsAndVertices + (vertCount+0) * 6);
		CVertex b(normalsAndVertices + (vertCount+1) * 6);
		CVertex c(normalsAndVertices + (vertCount+2) * 6);
		CTriangle3v tri(a,b,c);

		ChopTriangle3v(tri, plane, sliceResult);

		for(int i = 0; i < sliceResult.NormalSideCount; i++) {
			sliceResult.NormalSides[i][CTriangle3v::A].GetValue(&(bufN[bufNCount++][0]));
			sliceResult.NormalSides[i][CTriangle3v::B].GetValue(&(bufN[bufNCount++][0]));
			sliceResult.NormalSides[i][CTriangle3v::C].GetValue(&(bufN[bufNCount++][0]));
		}

		for(int i = 0; i < sliceResult.AntinormalSideCount; i++) {
			sliceResult.AntinormalSides[i][CTriangle3v::A].GetValue(&(bufA[bufACount++][0]));
			sliceResult.AntinormalSides[i][CTriangle3v::B].GetValue(&(bufA[bufACount++][0]));
			sliceResult.AntinormalSides[i][CTriangle3v::C].GetValue(&(bufA[bufACount++][0]));
		}

		if( 2 == sliceResult.InterSectionCount) {
			CLine line(sliceResult.Intersections[0].GetPoint(), sliceResult.Intersections[1].GetPoint());
			intersections.push_back(line);
		}
	}

	list<CTriangle3v> triangles;
	vector<CVector3f> contour;
	if(!GetContour(intersections, contour)) {
		return;
	}

	CVector3f refNormal = GetNormal(contour);

	while(contour.size() > 2) {
		for(int i = 0; i < contour.size()-1; i++) {
			int size = contour.size();
			int preIdx = (i+0)%size;
			int curIdx = (i+1)%size;
			int nxtIdx = (i+2)%size;
			if(CanSnip(preIdx, curIdx, nxtIdx, contour, refNormal)) {
				Snip(preIdx, curIdx, nxtIdx, contour, triangles);
				i = size; // break inner loop
			}
		}
	}

	list<CTriangle3v>::iterator it = triangles.begin();
	CVector3f normalN = plane[CPlane::N] * -1.0f;	// normal side face expected to face this direction
	CVector3f normalA = plane[CPlane::N];			// anti-normal side expected face direction

	bool samedirN = normalN.IsSameDirection(refNormal);
	bool samedirA = !samedirN;
	while(it != triangles.end()) {
		CTriangle3v tri = *it;
		if(bufNCount + 2 < 64) {
			tri.SetNormal(normalN);
			if(samedirN) {
				tri[CTriangle3v::A].GetValue(&(bufN[bufNCount++][0]));
				tri[CTriangle3v::B].GetValue(&(bufN[bufNCount++][0]));
				tri[CTriangle3v::C].GetValue(&(bufN[bufNCount++][0]));
			}
			else {
				tri[CTriangle3v::C].GetValue(&(bufN[bufNCount++][0]));
				tri[CTriangle3v::B].GetValue(&(bufN[bufNCount++][0]));
				tri[CTriangle3v::A].GetValue(&(bufN[bufNCount++][0]));
			}
		}
		if(bufACount + 2 < 64) {
			tri.SetNormal(normalA);
			if(samedirA) {
				tri[CTriangle3v::A].GetValue(&(bufA[bufACount++][0]));
				tri[CTriangle3v::B].GetValue(&(bufA[bufACount++][0]));
				tri[CTriangle3v::C].GetValue(&(bufA[bufACount++][0]));
			}
			else {
				tri[CTriangle3v::C].GetValue(&(bufA[bufACount++][0]));
				tri[CTriangle3v::B].GetValue(&(bufA[bufACount++][0]));
				tri[CTriangle3v::A].GetValue(&(bufA[bufACount++][0]));
			}
		}
		it++;
	}
}
