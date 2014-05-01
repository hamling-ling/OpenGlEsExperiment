#include "stdafx.h"

#include <list>
#include <vector>

#include "Chop.h"
#include "ChopInternal.h"

using namespace std;

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
void Chop(const MODELPLANE& plane, const float* normalsAndVertices, const int len,
		  float bufN[MAX_CHOP_BUF][8], float bufA[MAX_CHOP_BUF][8],
		  int& bufNCount, int& bufACount)
{
	bufNCount = 0;
	bufACount = 0;

	SliceResult3v sliceResult;
	list<MODELLINE> intersections;
#ifdef DEBUG
	vector<MODELVEC3D> debugVec;
#endif
	for(int vertCount = 0; vertCount < len; vertCount+=3)
	{
		MODELVERTEX a = MODELVERTEXMakeWithArray((float*)(normalsAndVertices + (vertCount+0) * 8));
		MODELVERTEX b = MODELVERTEXMakeWithArray((float*)normalsAndVertices + (vertCount+1) * 8);
		MODELVERTEX c = MODELVERTEXMakeWithArray((float*)normalsAndVertices + (vertCount+2) * 8);
		MODELTRIANGLE tri = {a,b,c};

		ChopTriangle3v(tri, plane, sliceResult);

		for(int i = 0; i < sliceResult.NormalSideCount && bufNCount+2 < MAX_CHOP_BUF; i++) {
			memcpy(&(bufN[bufNCount++][0]), &sliceResult.NormalSides[i].a, sizeof(MODELVERTEX));
			memcpy(&(bufN[bufNCount++][0]), &sliceResult.NormalSides[i].b, sizeof(MODELVERTEX));
			memcpy(&(bufN[bufNCount++][0]), &sliceResult.NormalSides[i].c, sizeof(MODELVERTEX));
		}

		for(int i = 0; i < sliceResult.AntinormalSideCount && bufACount+2 < MAX_CHOP_BUF; i++) {
			memcpy(&(bufA[bufACount++][0]), &sliceResult.AntinormalSides[i].a, sizeof(MODELVERTEX));
			memcpy(&(bufA[bufACount++][0]), &sliceResult.AntinormalSides[i].b, sizeof(MODELVERTEX));
			memcpy(&(bufA[bufACount++][0]), &sliceResult.AntinormalSides[i].c, sizeof(MODELVERTEX));
		}

		if( 2 == sliceResult.InterSectionCount) {

			MODELLINE line = {sliceResult.Intersections[0].point, sliceResult.Intersections[1].point};
			if(AddIntersection(line, intersections)) {
#ifdef DEBUG
				debugVec.push_back(line.a);
				debugVec.push_back(line.b);
#endif
			}
		}
	}

	list<vector<MODELVEC3D> > contours;
	if(!GetContours(intersections, contours)) {
		return;
	}

	list<vector<MODELVEC3D> >::iterator it = contours.begin();
	while(it != contours.end()) {
		ContourToVertices(*it, plane, bufN, bufA, bufNCount, bufACount);
		it++;
	}
}

/**
 * @brief Compute minimum box and center position and centering the vertex
 * @param in buf		vertex
 * @param in bufCount	vertex count
 * @param out box		minimum size box to compose the vertex
 * @param out cg        center of the box
 */
void AlignCenter(float buf[MAX_CHOP_BUF][8], const int bufCount, MODELVEC3D (&box)[8], MODELVEC3D &cg)
{
    if(bufCount <= 0)
        return;
    
    float minmax_x[2] = {buf[0][0]};
    float minmax_y[2] = {buf[0][1]};
    float minmax_z[2] = {buf[0][2]};
    for(int i = 1; i < bufCount && i < MAX_CHOP_BUF; i++) {
        minmax_x[0] = min(minmax_x[0], buf[i][0]);
        minmax_x[1] = max(minmax_x[1], buf[i][0]);
        minmax_y[0] = min(minmax_y[0], buf[i][1]);
        minmax_y[1] = max(minmax_y[1], buf[i][1]);
        minmax_z[0] = min(minmax_z[0], buf[i][2]);
        minmax_z[1] = max(minmax_z[1], buf[i][2]);
    }

    // 3D box
    box[0] = MODELVEC3DMake(minmax_x[0], minmax_y[0], minmax_z[0]);// min,min,min
    box[1] = MODELVEC3DMake(minmax_x[0], minmax_y[1], minmax_z[0]);// min,max,min
    box[2] = MODELVEC3DMake(minmax_x[1], minmax_y[1], minmax_z[0]);// max,max,min
	box[3] = MODELVEC3DMake(minmax_x[1], minmax_y[0], minmax_z[0]);// max,min,min
        
    box[4] = MODELVEC3DMake(minmax_x[0], minmax_y[0], minmax_z[1]);// min,min,max
    box[5] = MODELVEC3DMake(minmax_x[0], minmax_y[1], minmax_z[1]);// min,max,max
    box[6] = MODELVEC3DMake(minmax_x[1], minmax_y[1], minmax_z[1]);// max,max,max
    box[7] = MODELVEC3DMake(minmax_x[1], minmax_y[0], minmax_z[1]);// max,min,max
    
    // center
    cg = MODELVEC3DMake(
		(minmax_x[0]+ minmax_x[1])/2.0f,
        (minmax_y[0]+ minmax_y[1])/2.0f,
		(minmax_z[0]+ minmax_z[1])/2.0f
	);
    
    // centering vertex
    for(int i = 0; i < bufCount && i < MAX_CHOP_BUF; i++) {
        buf[i][0] -= cg.x;
        buf[i][1] -= cg.y;
        buf[i][2] -= cg.z;
    }
    
    // centering box
    for(int i = 0; i < 8; i++) {
        box[i] = SubtVec(box[i], cg);
    }
}

#pragma endregion
