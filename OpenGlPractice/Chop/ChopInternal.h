//
//  ChopInternal.h
//  ConsoleChop
//
//  Created by Nobuhiro Kuroiwa on 2014/05/01.
//  Copyright (c) 2014年 All rights reserved.
//

#ifndef ConsoleChop_ChopInternal_h
#define ConsoleChop_ChopInternal_h

#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include "float.h"

#include "Chop.h"
#include "CommonTool.h"

using namespace std;

#pragma region Prototypes

typedef struct SLICERESULT3v {
	int NormalSideCount;
	int AntinormalSideCount;
	MODELTRIANGLE NormalSides[2];
	MODELTRIANGLE AntinormalSides[2];
	MODELVERTEX Intersections[2];
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
	MODELLINE _line;
	MODELLINE _lineInv;
	
	IntersectionLineComparer(const MODELLINE& line )
	{
		_line = line;
		_lineInv = MODELLINEMake(line.b, line.a);
	}
	
	bool operator() (const MODELLINE &line)
	{
		bool equalsToLine = NearlyEquals(_line, line);
		if(equalsToLine)
			return true;
		
		bool equalsToLineInv = NearlyEquals(_lineInv, line);
		
		return equalsToLineInv;
	}
};

IntersectionType TryGetIntersection3v(const MODELVERTEX& r0, const MODELVEC3D& r1, const MODELVEC3D& n, const MODELVEC3D& p, MODELVERTEX& i0);

inline void Decompose3v(const MODELVERTEX &a, const MODELVERTEX &b, const MODELVERTEX &c,
				 const MODELVERTEX &i0, const MODELVERTEX &i1,
				 MODELTRIANGLE* tris);

inline bool ChopTriangle3v(const MODELTRIANGLE &tri, const MODELPLANE &plane, SliceResult3v& sliceResult);

inline bool FindConnection(const MODELVEC3D& p, const list<MODELLINE>& intersections, list<MODELLINE>::const_iterator& foundPos, MODELVEC3D& adjacent, MODELVEC3D& overlapped);

inline bool GetContours(const list<MODELLINE>& lines, list<vector<MODELVEC3D> >& contours);

inline bool GetContour(list<MODELLINE>& intersections, vector<MODELVEC3D>& contour);

inline bool TryGetConnection(const MODELVEC3D& p, const MODELLINE& line, MODELVEC3D& adjacent, MODELVEC3D& overlapped);

inline float GetArea(const vector<MODELLINE>& contour, const MODELLINE& n);

inline bool CanSnip(const int idxa, const int idxb, const int idxc,
			 const vector<MODELLINE>& closedIntersections,
			 const MODELLINE& normal);

inline bool IsInside(const int idxp, const int idxa, const int idxb, const int idxc,
			  const vector<MODELVEC3D>& closedIntersections);

inline void Snip(const int idxa, const int idxb, const int idxc,
		  vector<MODELLINE>& closedIntersections,
		  list<MODELTRIANGLE>& triangles);

inline bool AddIntersection(const MODELLINE& line, list<MODELLINE>& intersections);

inline void ContourToVertices(vector<MODELVEC3D> contour, const MODELPLANE& plane,
					   float bufN[MAX_CHOP_BUF][8], float bufA[MAX_CHOP_BUF][8],
					   int& bufNCount, int& bufACount);

#pragma endregion

#pragma region Definitions


inline bool OnThePlane(const MODELVEC3D& r0, const MODELVEC3D& r1, const MODELVEC3D& n, const MODELVEC3D& p)
{
	MODELVEC3D r0_p = SubtVec(p, r0);
	MODELVEC3D r1_p = SubtVec(p, r1);
	MODELVEC3D r0_p_cross_r1_p = CrossProduct(r0_p,r1_p);
	
	float n_cross_len = Magnitude(n) * Magnitude(r0_p_cross_r1_p);
	if(n_cross_len == 0.0)
		return false; // bug
	
	float cosT = DotProduct(n, r0_p_cross_r1_p);
	cosT /= n_cross_len;
	if(fabs(cosT) == 1.0)
		return true;
	else
		return false;
}


// ray(point r0 to point r1)
// plane(specified by normal and any point on the plane p)
// see http://www.thepolygoners.com/tutorials/lineplane/lineplane.html
inline IntersectionType TryGetIntersection3v(const MODELVERTEX& r0, const MODELVERTEX& r1, const MODELVEC3D& n, const MODELVEC3D& p, MODELVERTEX& i0)
{
	IntersectionType result = NoIntersection;
	
	const MODELVEC3D r0v = r0.point;
	MODELVEC3D r1v = r1.point;
	float NdotP2P0 = DotProduct(n, SubtVec(p, r0v));
	MODELVEC3D r0_r1 = SubtVec(r1v, r0v);
	float NdotV = DotProduct(n, r0_r1);
	
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
	
	MODELVEC3D i0v; // intersection
	if(0 < t && t < 1.0) {
		i0v = AddVec(r0v, ScaleVec(t, SubtVec(r1v, r0v)));
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
	float r0_i0_len = Magnitude(SubtVec(i0v, r0v));;
	float r0_r1_len = Magnitude(r0_r1);
	float ratio = r0_i0_len / r0_r1_len;
	
	ModelPoint t0 = r0.texcoord;
	ModelPoint t1 = r1.texcoord;
	ModelPoint tex = AddPoint(t0, ScalePoint(ratio, SubtPoint(t1, t0)));
	
	i0 = MODELVERTEXMake(i0v, r0.normal, tex);
	
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
inline void Decompose3(const MODELVERTEX &a, const MODELVERTEX &b, const MODELVERTEX &c,
				const MODELVERTEX &i0, const MODELVERTEX &i1,
				MODELTRIANGLE* tris)
{
	tris[0] = MODELTRIANGLEMake(a, i0, i1);
	tris[1] = MODELTRIANGLEMake(b, c,  i0);
	tris[2] = MODELTRIANGLEMake(c, i1, i0);
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
inline void Decompose2(const MODELVERTEX &a, const MODELVERTEX &b, const MODELVERTEX &c,
				const MODELVERTEX &i1, MODELTRIANGLE* tris)
{
	tris[0] = MODELTRIANGLEMake(a, b, i1);
	tris[1] = MODELTRIANGLEMake(a, i1, c);
}


/**
 test which side of a plane a given triangle is belonged to.
 @param tri triangle
 @param n normal vector of a plane
 @param p any point on a plane
 @param sliceResult result
 */
inline void SortSides(const MODELTRIANGLE &tri, const MODELVEC3D& n, const MODELVEC3D& p, SliceResult3v& sliceResult)
{
	MODELVEC3D a = GetCenter(tri);
	MODELVEC3D p_a = SubtVec(a, p);
	
	float denominator = Magnitude(p_a) * Magnitude(n);
	if(denominator != 0.0) {
		float numerator = DotProduct(p_a, n);
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


inline bool ChopTriangle3v(const MODELTRIANGLE &tri, const MODELPLANE &plane, SliceResult3v& sliceResult)
{
	MODELVERTEX i0,i1;
	MODELVEC3D n = plane.normal;
	MODELVEC3D p = plane.point;
	MODELTRIANGLE decomped[3];
	
	int decompedLen = 1;
	decomped[0] = tri;
	memset(&sliceResult, 0, sizeof(sliceResult));
	
	bool result = false;
	for(int i = 0; i < 3; i++) {
		MODELVERTEX A = tri.v[(i+0)%3];
		MODELVERTEX B = tri.v[(i+1)%3];
		MODELVERTEX C = tri.v[(i+2)%3];
		
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


inline bool FindConnection(const MODELVEC3D& p, const list<MODELLINE>& intersections, list<MODELLINE>::const_iterator& foundPos, MODELVEC3D& adjacent, MODELVEC3D& overlapped)
{
	list<MODELLINE>::const_iterator it = intersections.begin();
	while(it != intersections.end()) {
		if(TryGetConnection(p, *it, adjacent, overlapped)) {
			foundPos = it;
			return true;
		}
		it++;
	}
	return false;
}


inline bool GetContours(const list<MODELLINE>& lines, list<vector<MODELVEC3D> >& contours)
{
	list<MODELLINE> intersections(lines.begin(), lines.end());
	vector<MODELVEC3D> contour;
	
	while(GetContour(intersections, contour)) {
		contours.push_back(contour);
	}
	
	return !contours.empty();
}


inline bool GetContour(list<MODELLINE>& intersections, vector<MODELVEC3D>& contour)
{
	contour.clear();
	
	if(intersections.size() < 3) {
		return false;
	}
	
	list<MODELLINE>::iterator it = intersections.begin();
	contour.push_back(it->a);
	contour.push_back(it->b);
	intersections.pop_front();
	
	MODELVEC3D adjacent, overlapped;
	
	while(intersections.size() > 0) {
		
		list<MODELLINE>::const_iterator foundPos;
		if(FindConnection(*(contour.rbegin()), intersections, foundPos, adjacent, overlapped)) {
			
			contour.push_back(adjacent);
			intersections.erase(foundPos);
		}
		else {
			break;
		}
	}
	
	// if it is closed
	vector<MODELVEC3D>::const_reverse_iterator last = contour.rbegin();
	if( contour.size() > 2 &&
	   NearlyEquals(*contour.begin(), *last)) {
		// yes, it's closed
		contour.pop_back();
	}
	else {
		contour.clear();
	}
	
	return !contour.empty();
}


inline bool TryGetConnection(const MODELVEC3D& p, const MODELLINE& line, MODELVEC3D& adjacent, MODELVEC3D& overlapped)
{
	if(NearlyEquals(line.a, p)) {
		adjacent = line.b;
		overlapped = line.a;
		return true;
	}
	else if(NearlyEquals(line.b, p)) {
		adjacent = line.a;
		overlapped = line.b;
		return true;
	}
	return false;
}


inline float GetArea(const vector<MODELVEC3D>& contour, const MODELVEC3D& n)
{
	const int size = static_cast<int>(contour.size());
	MODELVEC3D normal = ZEROVEC3D;
	for(int i = 0; i < size; i++) {
		int idxa = (i+0)%size; // a
		int idxb = (i+1)%size; // b
		MODELVEC3D vi1 = contour[idxa];
		MODELVEC3D vi2 = contour[idxb];
		normal = AddVec(CrossProduct(vi1, vi2), normal);
	}
	
	return DotProduct(normal,n);
}


inline bool CanSnip(const int idxa, const int idxb, const int idxc,
			 const vector<MODELVEC3D>& contour,
			 const MODELVEC3D& normal)
{
	MODELVEC3D B = SubtVec(contour[idxa], contour[idxb]);
	MODELVEC3D A = SubtVec(contour[idxc], contour[idxb]);
	MODELVEC3D AxB=CrossProduct(A, B);
	MODELVEC3D AxBNorm = Normalize(AxB);
	
	if(!FEQ(Magnitude(AxB), 0.0)) {
		if(!IsSameDirection(AxBNorm, normal)) {
			return false;
		}
	}
	
	int size = static_cast<int>(contour.size());
	for(int idx = 0; idx < size; idx++) {
		if(idx == idxa || idx == idxb || idx == idxc) {
			continue;
		}
		if(IsInside(idx, idxa, idxb, idxc, contour)) {
			return false;
		}
	}
	
	return true;
}


inline bool IsInside(const int idxp, const int idxa, const int idxb, const int idxc, const vector<MODELVEC3D>& contour)
{
	MODELVEC3D ab = SubtVec(contour[idxb], contour[idxa]);
	MODELVEC3D bc = SubtVec(contour[idxc], contour[idxb]);
	MODELVEC3D ca = SubtVec(contour[idxa], contour[idxc]);
	MODELVEC3D ap = SubtVec(contour[idxp], contour[idxa]);
	MODELVEC3D bp = SubtVec(contour[idxp], contour[idxb]);
	MODELVEC3D cp = SubtVec(contour[idxp], contour[idxc]);
	
	MODELVEC3D e1 = CrossProduct(ab, ap);
	MODELVEC3D e2 = CrossProduct(bc, bp);
	MODELVEC3D e3 = CrossProduct(ca, cp);
	
	float cosT1 = DotProduct(e1, e2);
	float cosT2 = DotProduct(e2, e3);
	float cosT3 = DotProduct(e3, e1);
	
	if(ISPOSITIVE(cosT1) && ISPOSITIVE(cosT2) && ISPOSITIVE(cosT3)) {
		return true;
	}
	
	return false;
}


inline void Snip(const int idxa, const int idxb, const int idxc,
		  vector<MODELVEC3D>& contour,
		  list<MODELTRIANGLE>& triangles)
{
	MODELVEC3D normal = ZEROVEC3D; // temporarily set something
	MODELVERTEX va = {contour[idxa], normal, ZEROPOINT};
	MODELVERTEX vb = {contour[idxb], normal, ZEROPOINT};
	MODELVERTEX vc = {contour[idxc], normal, ZEROPOINT};
	
	MODELTRIANGLE tri = {va, vb, vc};
	triangles.push_back(tri);
	
	contour.erase(contour.begin() + idxb);
}


inline void ContourToVertices(vector<MODELVEC3D> contour, const MODELPLANE& plane,
					   float bufN[MAX_CHOP_BUF][8], float bufA[MAX_CHOP_BUF][8],
					   int& bufNCount, int& bufACount)
{
	// make contour clockwise around given plane's normal
	float area = GetArea(contour, plane.normal);
	if(area < 0.0) {
		vector<MODELVEC3D> vec;
		vec.assign(contour.rbegin(), contour.rend());
		contour = vec;
	}
	
	list<MODELTRIANGLE> triangles;
	int size = static_cast<int>(contour.size());
	while(size > 2) {
		
		int sizeBeforeSnip = size;
		
		for(int i = 0; i < size; i++) {
			int preIdx = (i+0)%size;
			int curIdx = (i+1)%size;
			int nxtIdx = (i+2)%size;
			if(CanSnip(preIdx, curIdx, nxtIdx, contour, plane.normal)) {
				Snip(preIdx, curIdx, nxtIdx, contour, triangles);
				size = static_cast<int>(contour.size());
				break; // break inner loop
			}
		}
		
		if(size == sizeBeforeSnip) {
			// nothing snipped
			break;
		}
	}
	
	list<MODELTRIANGLE>::iterator it = triangles.begin();
	MODELVEC3D normalN = ScaleVec(-1.0, plane.normal);	// normal side face expected to face this direction
	MODELVEC3D normalA = plane.normal;			// anti-normal side expected face direction
	
	while(it != triangles.end()) {
		MODELTRIANGLE tri = *it;
		if(bufNCount + 2 < MAX_CHOP_BUF) {
			SetNormal(tri, normalN);
			memcpy(&(bufN[bufNCount++][0]), &tri.c, sizeof(MODELVERTEX));
			memcpy(&(bufN[bufNCount++][0]), &tri.b, sizeof(MODELVERTEX));
			memcpy(&(bufN[bufNCount++][0]), &tri.a, sizeof(MODELVERTEX));
		}
		if(bufACount + 2 < MAX_CHOP_BUF) {
			SetNormal(tri, normalA);
			memcpy(&(bufA[bufACount++][0]), &tri.a, sizeof(MODELVERTEX));
			memcpy(&(bufA[bufACount++][0]), &tri.b, sizeof(MODELVERTEX));
			memcpy(&(bufA[bufACount++][0]), &tri.c, sizeof(MODELVERTEX));
		}
		it++;
	}
}

inline bool AddIntersection(const MODELLINE& line, list<MODELLINE>& intersections)
{
	if(std::find_if(intersections.begin(), intersections.end(), IntersectionLineComparer(line))
	   != intersections.end()) {
		// dupulication is not allowed
		return false;
	}
	
	intersections.push_back(line);
	
	return true;
}


#endif
