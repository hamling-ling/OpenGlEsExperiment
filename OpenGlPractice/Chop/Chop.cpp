#include "stdafx.h"

#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include "float.h"

// debug
#include <Windows.h>

#include "Chop.h"
#include "Vector3f.h"
#include "Vertex.h"
#include "Line.h"

using namespace std;

#pragma region Prototypes

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

struct IntersectionLineComparer {
	CLine _line;
	CLine _lineInv;

	IntersectionLineComparer(const CLine& line )
	{
		_line = line;
		_lineInv = CLine(line[CLine::B], line[CLine::A]);
	}

	bool operator() (const CLine &line)
	{
		bool equalsToLine = _line[CLine::A].NearlyEquals(line[CLine::A]) &&
							_line[CLine::B].NearlyEquals(line[CLine::B]);
		if(equalsToLine)
			return true;

		bool equalsToLineInv = _lineInv[CLine::A].NearlyEquals(line[CLine::A]) &&
							   _lineInv[CLine::B].NearlyEquals(line[CLine::B]);

		return equalsToLineInv;
	}
};

IntersectionType TryGetIntersection3v(const CVertex& r0, const CVertex& r1, const CVector3f& n, const CVector3f& p, CVertex& i0);

void Decompose3v(const CVertex &a, const CVertex &b, const CVertex &c,
				 const CVertex &i0, const CVertex &i1,
				 CTriangle3v* tris);

bool ChopTriangle3v(const CTriangle3v &tri, const CPlane &plane, SliceResult3v& sliceResult);

bool FindConnection(const CVector3f& p, const list<CLine>& intersections, list<CLine>::const_iterator& foundPos, CVector3f& adjacent, CVector3f& overlapped);

bool GetContours(const list<CLine>& lines, list<vector<CVector3f> >& contours);

bool GetContour(list<CLine>& intersections, vector<CVector3f>& contour);

bool TryGetConnection(const CVector3f& p, const CLine& line, CVector3f& adjacent, CVector3f& overlapped);

float GetArea(const vector<CVector3f>& contour, const CVector3f& n);

bool CanSnip(const int idxa, const int idxb, const int idxc,
			 const vector<CVector3f>& closedIntersections,
			 const CVector3f& normal);

bool IsInside(const int idxp, const int idxa, const int idxb, const int idxc,
		  const vector<CVector3f>& closedIntersections);

void Snip(const int idxa, const int idxb, const int idxc,
		  vector<CVector3f>& closedIntersections,
		  list<CTriangle3v>& triangles);

bool AddIntersection(const CLine& line, list<CLine>& intersections);

void ContourToVertices(vector<CVector3f> contour, const CPlane& plane,
					 float bufN[MAX_CHOP_BUF][8], float bufA[MAX_CHOP_BUF][8],
					 int& bufNCount, int& bufACount);

#pragma endregion


#pragma region Definitions


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

	// compute texture location
	float r0_i0_len = (i0v - r0v).Length();
	float r0_r1_len = r0_r1.Length();
	float ratio = r0_i0_len / r0_r1_len;

	CVector2f t0 = r0.GetTex();
	CVector2f t1 = r1.GetTex();
	CVector2f tex = t0 + ((t1 - t0) * ratio);
	i0.SetValue(i0v, r0.GetNormal(), tex);

	return result;
}


/**
 * @bfief decompose a triangle into three triangles.
 * given triangle consists of three points a,b,c and two intersections
 * i0 and i1. i0 should be between a and b. i1 also be between b and c.
 *
 * a-----i0-----b
 *  \          /
 *   \        /
 *    i1     /
 *      \   /
         \ /
 *        c
 */
void Decompose3(const CVertex &a, const CVertex &b, const CVertex &c,
				const CVertex &i0, const CVertex &i1,
				CTriangle3v* tris)
{
	tris[0].SetValue(a, i0, i1);
	tris[1].SetValue(b, c, i0);
	tris[2].SetValue(c, i1, i0);
}


/**
 * @bfief decompose a triangle into two triangles.
 * given triangle consists of three points a,b,c and two intersections
 * a and i1. i1 should be between b and c.
 *
 *   a
 *  /  \
 * /    \
 *c--i1--b
 */
void Decompose2(const CVertex &a, const CVertex &b, const CVertex &c,
				const CVertex &i1, CTriangle3v* tris)
{
	tris[0].SetValue(a, b, i1);
	tris[1].SetValue(a, i1, c);
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
	CVector3f n = (plane[CPlane::N]);
	CVector3f p = (plane[CPlane::P]);
	CTriangle3v decomped[3];

	int decompedLen = 1;
	decomped[0] = tri;
	memset(&sliceResult, 0, sizeof(sliceResult));

	bool result = false;
	for(int i = 0; i < 3; i++) {
		CVertex A = tri[(i+0)%3];
		CVertex B = tri[(i+1)%3];
		CVertex C = tri[(i+2)%3];

		IntersectionType firstResult = TryGetIntersection3v(A,B,n,p,i0);
		if(BetweenEnds == firstResult) {
			IntersectionType secondResult = TryGetIntersection3v(B,C,n,p,i1);
			if(BetweenEnds == secondResult) {
				Decompose3( B, C, A, i1, i0, decomped);
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
			IntersectionType secondResult = TryGetIntersection3v(B,C,n,p,i1);
			if(BetweenEnds == secondResult) {
				Decompose2( A, B, C, i1, decomped);
				decompedLen = 2;
				result = true;
				// save intersection
				sliceResult.Intersections[0] = A;
				sliceResult.Intersections[1] = i1;
				sliceResult.InterSectionCount = 2;
				break;
			}
		}
		else if(OnParallel == firstResult) {
			// save intersection
			sliceResult.Intersections[0] = A;
			sliceResult.Intersections[1] = B;
			sliceResult.InterSectionCount = 2;
			break;
		}
	}

	// determine a side and set result
	for(int i = 0; i < decompedLen; i++) {
		SortSides(decomped[i], n, p, sliceResult);
	}

	return result;
}


bool FindConnection(const CVector3f& p, const list<CLine>& intersections, list<CLine>::const_iterator& foundPos, CVector3f& adjacent, CVector3f& overlapped)
{
	list<CLine>::const_iterator it = intersections.begin();
	while(it != intersections.end()) {
		if(TryGetConnection(p, *it, adjacent, overlapped)) {
			foundPos = it;
			return true;
		}
		it++;
	}
	return false;
}


bool GetContours(const list<CLine>& lines, list<vector<CVector3f> >& contours)
{
	list<CLine> intersections(lines.begin(), lines.end());
	vector<CVector3f> contour;
	bool found = false;

	while(GetContour(intersections, contour)) {
		contours.push_back(contour);
	}

	return !contours.empty();
}


bool GetContour(list<CLine>& intersections, vector<CVector3f>& contour)
{
	contour.clear();

	if(intersections.size() < 3) {
		return false;
	}

	list<CLine>::iterator it = intersections.begin();
	contour.push_back((*it)[CLine::A]);
	contour.push_back((*it)[CLine::B]);
	intersections.pop_front();

	CVector3f adjacent, overlapped;

	while(intersections.size() > 0) {

		CVector3f connection, nonconnection;
		list<CLine>::const_iterator foundPos;
		if(FindConnection(*(contour.rbegin()), intersections, foundPos, adjacent, overlapped)) {

			contour.push_back(adjacent);
			intersections.erase(foundPos);
		}
		else {
			break;
		}
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


bool TryGetConnection(const CVector3f& p, const CLine& line, CVector3f& adjacent, CVector3f& overlapped)
{
	if(line[CLine::A].NearlyEquals(p)) {
		adjacent = line[CLine::B];
		overlapped = line[CLine::A];
		return true;
	}
	else if(line[CLine::B].NearlyEquals(p)) {
		adjacent = line[CLine::A];
		overlapped = line[CLine::B];
		return true;
	}
	return false;
}


float GetArea(const vector<CVector3f>& contour, const CVector3f& n)
{
	const int size = static_cast<int>(contour.size());
	CVector3f normal;
	for(int i = 0; i < size; i++) {
		int idxa = (i+0)%size; // a
		int idxb = (i+1)%size; // b
		CVector3f vi1 = contour[idxa];
		CVector3f vi2 = contour[idxb];
		normal += vi1.Cross(vi2);
	}

	return normal.Dot(n);
}


bool CanSnip(const int idxa, const int idxb, const int idxc,
			 const vector<CVector3f>& contour,
			 const CVector3f& normal)
{
	CVector3f B = contour[idxa] - contour[idxb];
	CVector3f A = contour[idxc] - contour[idxb];
	CVector3f AxB=A.Cross(B);
	CVector3f AxBNorm = AxB;
	AxBNorm.Normalize();

	if(!FEQ(AxB.Length(), 0.0)) {
		if(!AxBNorm.IsSameDirection(normal)) {
			return false;
		}
	}

	int size = contour.size();
	for(int idx = 0; idx < size-1; idx++) {
		if(idx == idxa || idx == idxb || idx == idxc) {
			continue;
		}
		if(IsInside(idx, idxa, idxb, idxc, contour)) {
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
	CVector3f normal; // temporarily set something
	CVertex va( contour[idxa], normal, CVector2f());
	CVertex vb( contour[idxb], normal, CVector2f());
	CVertex vc( contour[idxc], normal, CVector2f());

	CTriangle3v tri(va, vb, vc);
	triangles.push_back(tri);

	contour.erase(contour.begin() + idxb);
}


void ContourToVertices(vector<CVector3f> contour, const CPlane& plane,
					 float bufN[MAX_CHOP_BUF][8], float bufA[MAX_CHOP_BUF][8],
					 int& bufNCount, int& bufACount)
{
	// make contour clockwise around given plane's normal
	float area = GetArea(contour, plane[CPlane::N]);
	if(area < 0.0) {
		vector<CVector3f> vec;
		vec.assign(contour.rbegin(), contour.rend());
		contour = vec;
	}

	list<CTriangle3v> triangles;
	while(contour.size() > 2) {
		for(int i = 0; i < contour.size()-1; i++) {
			int size = contour.size();
			int preIdx = (i+0)%size;
			int curIdx = (i+1)%size;
			int nxtIdx = (i+2)%size;
			if(CanSnip(preIdx, curIdx, nxtIdx, contour, plane[CPlane::N])) {
				Snip(preIdx, curIdx, nxtIdx, contour, triangles);
				i = size; // break inner loop
			}
		}
	}

	list<CTriangle3v>::iterator it = triangles.begin();
	CVector3f normalN = plane[CPlane::N] * -1.0f;	// normal side face expected to face this direction
	CVector3f normalA = plane[CPlane::N];			// anti-normal side expected face direction

	while(it != triangles.end()) {
		CTriangle3v tri = *it;
		if(bufNCount + 2 < MAX_CHOP_BUF) {
			tri.SetNormal(normalN);
			tri[CTriangle3v::C].GetValue(&(bufN[bufNCount++][0]));
			tri[CTriangle3v::B].GetValue(&(bufN[bufNCount++][0]));
			tri[CTriangle3v::A].GetValue(&(bufN[bufNCount++][0]));
		}
		if(bufACount + 2 < MAX_CHOP_BUF) {
			tri.SetNormal(normalA);
			tri[CTriangle3v::A].GetValue(&(bufA[bufACount++][0]));
			tri[CTriangle3v::B].GetValue(&(bufA[bufACount++][0]));
			tri[CTriangle3v::C].GetValue(&(bufA[bufACount++][0]));
		}
		it++;
	}
}

bool AddIntersection(const CLine& line, list<CLine>& intersections)
{
	if(std::find_if(intersections.begin(), intersections.end(), IntersectionLineComparer(line))
		!= intersections.end()) {
			// dupulication is not allowed
			return false;
	}

	intersections.push_back(line);

	return true;
}

/**
 * @brief Cut a mesh with plane.
 * @param in plane					intersecting plane
 * @param in normalsAndVertices		mesh to be cut
 * @param len						num of vertex in the normalsandVertices
 * @param bufN	resulting vertex that locating normal side of the plane
 * @param bufA	resulting vertex that locating anti-normal side of the plane
 * @param bufNCount	num of vertices in bufN
 * @param bufACount num of vertices in bufA
 */
void Chop(const CPlane& plane, const float* normalsAndVertices, const int len,
		  float bufN[MAX_CHOP_BUF][8], float bufA[MAX_CHOP_BUF][8],
		  int& bufNCount, int& bufACount)
{
	bufNCount = 0;
	bufACount = 0;

	SliceResult3v sliceResult;
	list<CLine> intersections;
#ifdef DEBUG
	vector<CVector3f> debugVec;
#endif
	for(int vertCount = 0; vertCount < len; vertCount+=3)
	{
		CVertex a(normalsAndVertices + (vertCount+0) * 8);
		CVertex b(normalsAndVertices + (vertCount+1) * 8);
		CVertex c(normalsAndVertices + (vertCount+2) * 8);
		CTriangle3v tri(a,b,c);

		ChopTriangle3v(tri, plane, sliceResult);

		for(int i = 0; i < sliceResult.NormalSideCount && bufNCount+2 < MAX_CHOP_BUF; i++) {
			sliceResult.NormalSides[i][CTriangle3v::A].GetValue(&(bufN[bufNCount++][0]));
			sliceResult.NormalSides[i][CTriangle3v::B].GetValue(&(bufN[bufNCount++][0]));
			sliceResult.NormalSides[i][CTriangle3v::C].GetValue(&(bufN[bufNCount++][0]));
		}

		for(int i = 0; i < sliceResult.AntinormalSideCount && bufACount+2 < MAX_CHOP_BUF; i++) {
			sliceResult.AntinormalSides[i][CTriangle3v::A].GetValue(&(bufA[bufACount++][0]));
			sliceResult.AntinormalSides[i][CTriangle3v::B].GetValue(&(bufA[bufACount++][0]));
			sliceResult.AntinormalSides[i][CTriangle3v::C].GetValue(&(bufA[bufACount++][0]));
		}

		if( 2 == sliceResult.InterSectionCount) {

			CLine line(sliceResult.Intersections[0].GetPoint(), sliceResult.Intersections[1].GetPoint());
			if(AddIntersection(line, intersections)) {
#ifdef DEBUG
				debugVec.push_back(line[CLine::A]);
				debugVec.push_back(line[CLine::B]);
#endif
			}
		}
	}

	list<vector<CVector3f> > contours;
	if(!GetContours(intersections, contours)) {
		return;
	}

	list<vector<CVector3f> >::iterator it = contours.begin();
	while(it != contours.end()) {
		ContourToVertices(*it, plane, bufN, bufA, bufNCount, bufACount);
		it++;
	}
}

#pragma endregion
