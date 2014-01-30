#pragma once


#include "Triangle3v.h"
#include "Plane.h"

#define MAX_CHOP_BUF	8192

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
		  int& bufNCount, int& bufACount);

/**
 * @brief Compute minimum box and center position and centering the vertex
 * @param in buf		vertex
 * @param in bufCount	vertex count
 * @param out box		minimum size box to compose the vertex
 * @param out cg        center of the box
 */
void AlignCenter(float buf[MAX_CHOP_BUF][8], const int bufCount, CVector3f (&box)[8], CVector3f &cg);