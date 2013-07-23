#include "stdafx.h"
#include "CppUnitTest.h"

#include <list>
#include <vector>
#include "Slice.h"
#include "Plane.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SliceTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			const float normalsAndVertices[3][6] =
			{
				{0.0, 0.0, 1.0, -0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f,  0.5f,  0.5f},
			};

			CVertex a(normalsAndVertices[0]);
			CVertex b(normalsAndVertices[1]);
			CVertex c(normalsAndVertices[2]);
			CTriangle3v tri(a,b,c);

			CVector3f p(-0.1f, 1.0f, 0.0f);
			CVector3f n(1.0f, 0.1f, 0.0f);
			CPlane plane(n, p);

			SliceResult3v sliced;
			bool isSliced = SliceTriangle3v(tri, plane, sliced);

			Assert::IsTrue(isSliced);
			Assert::IsTrue(sliced.NormalSideCount == 2);
			Assert::IsTrue(sliced.AntinormalSideCount == 1);

			// 1st triangle
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VY],-0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VY], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VX], 0.05));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));

			// 2nd triangle
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::A][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::A][CVertex::VY], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::B][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::B][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::C][CVertex::VX], 0.05));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::C][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::C][CVertex::VZ], 0.5));

			// 3rd triangle
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VX],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VY],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));
			
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VX],0.05));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VY],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));
		}

		TEST_METHOD(TestMethod2)
		{
			const float normalsAndVertices[3][6] =
			{
				{0.0, 0.0, 1.0, -0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f,  0.5f,  0.5f},
			};

			CVertex a(normalsAndVertices[0]);
			CVertex b(normalsAndVertices[1]);
			CVertex c(normalsAndVertices[2]);
			CTriangle3v tri(a,b,c);

			CVector3f p(-1.0f, 0.0f, 0.0f);
			CVector3f n(0.0f, 1.0f, 0.0f);
			CPlane plane(n, p);

			SliceResult3v sliced;
			bool isSliced = SliceTriangle3v(tri, plane, sliced);

			Assert::IsTrue(isSliced);
			Assert::IsTrue(sliced.NormalSideCount == 1);
			Assert::IsTrue(sliced.AntinormalSideCount == 2);
			
			// 1st triangle
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VY], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));

			//// 2nd triangle
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VX], -0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VY],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));

			//// 3rd triangle
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::A][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::A][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::B][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::B][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::C][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::C][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[1][CTriangle3v::C][CVertex::VZ], 0.5));
		}

		TEST_METHOD(TestMethod3)
		{
			const float normalsAndVertices[3][6] =
			{
				{0.0, 0.0, 1.0, -0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f,  0.5f,  0.5f},
			};

			CVertex a(normalsAndVertices[0]);
			CVertex b(normalsAndVertices[1]);
			CVertex c(normalsAndVertices[2]);
			CTriangle3v tri(a,b,c);

			CVector3f p(0.0f, -0.5f, 0.0f);
			CVector3f n((float)(-1.0/sqrt(2.0)), (float)(1.0/sqrt(2.0)), 0.0f);
			CPlane plane(n, p);

			SliceResult3v sliced;
			bool isSliced = SliceTriangle3v(tri, plane, sliced);

			Assert::IsTrue(isSliced);
			Assert::IsTrue(sliced.NormalSideCount == 2);
			Assert::IsTrue(sliced.AntinormalSideCount == 1);

			// 1st triangle
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VY], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VX], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));

			// 2nd triangle
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::A][CVertex::VX], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::A][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::B][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::B][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::C][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::C][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.NormalSides[1][CTriangle3v::C][CVertex::VZ], 0.5));

			// 3rd triangle
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VY], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VX], 0.0));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VY], -0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));
		}

		TEST_METHOD(TestMethod4)
		{
			const float normalsAndVertices[3][6] =
			{
				{0.0, 0.0, 1.0, -0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f, -0.5f,  0.5f},
				{0.0, 0.0, 1.0,  0.5f,  0.5f,  0.5f},
			};

			CVertex a(normalsAndVertices[0]);
			CVertex b(normalsAndVertices[1]);
			CVertex c(normalsAndVertices[2]);
			CTriangle3v tri(a,b,c);

			CVector3f p(-0.5f, -0.5f, 0.0f);
			CVector3f n(0.0f, -1.0f, 0.0f);
			CPlane plane(n, p);

			SliceResult3v sliced;
			bool isSliced = SliceTriangle3v(tri, plane, sliced);

			Assert::IsFalse(isSliced);
			Assert::IsTrue(sliced.NormalSideCount == 0);
			Assert::IsTrue(sliced.AntinormalSideCount == 1);

			// 1st triangle
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VX],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VY],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::A][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VY],-0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::B][CVertex::VZ], 0.5));

			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VX], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VY], 0.5));
			Assert::IsTrue(FEQ(sliced.AntinormalSides[0][CTriangle3v::C][CVertex::VZ], 0.5));
		}

		TEST_METHOD(TestTriangulation)
		{
			const float normalsAndVertices[][6] =
			{
				/* ëO */
				{ 0.0f,  0.0f,  1.0f, -0.5f, -0.5f,  0.5f},
				{ 0.0f,  0.0f,  1.0f,  0.5f, -0.5f,  0.5f},
				{ 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f},

				{ 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f},
				{ 0.0f,  0.0f,  1.0f, -0.5f,  0.5f,  0.5f},
				{ 0.0f,  0.0f,  1.0f, -0.5f, -0.5f,  0.5f},

				/* å„ */
				{ 0.0f,  0.0f, -1.0f,  0.5f, -0.5f, -0.5f},
				{ 0.0f,  0.0f, -1.0f, -0.5f, -0.5f, -0.5f},
				{ 0.0f,  0.0f, -1.0f, -0.5f,  0.5f, -0.5f},

				{ 0.0f,  0.0f, -1.0f, -0.5f,  0.5f, -0.5f},
				{ 0.0f,  0.0f, -1.0f,  0.5f,  0.5f, -0.5f},
				{ 0.0f,  0.0f, -1.0f,  0.5f, -0.5f, -0.5f},

				/* âE */
				{ 1.0f,  0.0f,  0.0f,  0.5f, -0.5f,  0.5f},
				{ 1.0f,  0.0f,  0.0f,  0.5f, -0.5f, -0.5f},
				{ 1.0f,  0.0f,  0.0f,  0.5f,  0.5f, -0.5f},

				{ 1.0f,  0.0f,  0.0f,  0.5f,  0.5f, -0.5f},
				{ 1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f},
				{ 1.0f,  0.0f,  0.0f,  0.5f, -0.5f,  0.5f},

				/* ç∂ */
				{-1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f},
				{-1.0f,  0.0f,  0.0f, -0.5f, -0.5f,  0.5f},
				{-1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f},

				{-1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f},
				{-1.0f,  0.0f,  0.0f, -0.5f,  0.5f, -0.5f},
				{-1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f},

				/* è„ */
				{ 0.0f,  1.0f,  0.0f, -0.5f,  0.5f,  0.5f},
				{ 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f},
				{ 0.0f,  1.0f,  0.0f,  0.5f,  0.5f, -0.5f},

				{ 0.0f,  1.0f,  0.0f,  0.5f,  0.5f, -0.5f},
				{ 0.0f,  1.0f,  0.0f, -0.5f,  0.5f, -0.5f},
				{ 0.0f,  1.0f,  0.0f, -0.5f,  0.5f,  0.5f},

				/* â∫ */
				{ 0.0f, -1.0f,  0.0f,  0.5f, -0.5f,  0.5f},
				{ 0.0f, -1.0f,  0.0f, -0.5f, -0.5f,  0.5f},
				{ 0.0f, -1.0f,  0.0f, -0.5f, -0.5f, -0.5f},

				{ 0.0f, -1.0f,  0.0f, -0.5f, -0.5f, -0.5f},
				{ 0.0f, -1.0f,  0.0f,  0.5f, -0.5f, -0.5f},
				{ 0.0f, -1.0f,  0.0f,  0.5f, -0.5f,  0.5f}
			};

			int len = sizeof(normalsAndVertices)/6/sizeof(float);
			float bufN[64][6] = {0.0f};
			float bufA[64][6] = {0.0f};
			int bufNCount = 0;
			int bufACount = 0;

			CVector3f p(-0.1f, 1.0f, 0.0f);
			CVector3f n(1.0f, 0.1f, 0.0f);
			CPlane plane(n, p);

			SliceResult3v sliceResult;
			list<CLine> intersections;
			for(int vertCount = 0; vertCount < len; vertCount+=3)
			{
				CVertex a(normalsAndVertices[vertCount+0]);
				CVertex b(normalsAndVertices[vertCount+1]);
				CVertex c(normalsAndVertices[vertCount+2]);
				CTriangle3v tri(a,b,c);

				SliceTriangle3v(tri, plane, sliceResult);

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
			vector<CVector3f> closedIntersections;
			if(!GetClosedIntersections(intersections, closedIntersections)) {
				return;
			}

			CVector3f refNormal = GetNormal(closedIntersections);

			while(closedIntersections.size() > 2) {
				for(int i = 0; i < closedIntersections.size()-1; i++) {
					int size = closedIntersections.size();
					int preIdx = (i+0)%size;
					int curIdx = (i+1)%size;
					int nxtIdx = (i+2)%size;
					if(CanSnip(preIdx, curIdx, nxtIdx, closedIntersections, refNormal)) {
						Snip(preIdx, curIdx, nxtIdx, closedIntersections, triangles);
						i = size; // break inner loop
					}
				}
			}

			list<CTriangle3v>::iterator it = triangles.begin();
			CVector3f normal = n;
			CVector3f antnormal = n * 1.0f;

			Assert::IsTrue(triangles.size() == 6);
			while(it != triangles.end()) {
				Assert::IsTrue(bufNCount + 2 < 64);

				CTriangle3v tri = *it;
				tri.SetNormal(antnormal);
				tri[CTriangle3v::A].GetValue(&(bufN[bufNCount++][0]));
				tri[CTriangle3v::B].GetValue(&(bufN[bufNCount++][0]));
				tri[CTriangle3v::C].GetValue(&(bufN[bufNCount++][0]));

				Assert::IsTrue(bufACount + 2 < 64);
				tri.SetNormal(n);
				tri[CTriangle3v::A].GetValue(&(bufA[bufACount++][0]));
				tri[CTriangle3v::B].GetValue(&(bufA[bufACount++][0]));
				tri[CTriangle3v::C].GetValue(&(bufA[bufACount++][0]));
				it++;
			}
		}
	};
}