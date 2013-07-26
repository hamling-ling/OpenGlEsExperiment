#include "stdafx.h"
#include "CppUnitTest.h"

#include <list>
#include <vector>
#include "Chop.h"
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
			const float normalsAndVertices[3][8] =
			{
				{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			};

			CVertex a(normalsAndVertices[0]);
			CVertex b(normalsAndVertices[1]);
			CVertex c(normalsAndVertices[2]);
			CTriangle3v tri(a,b,c);

			CVector3f p(-0.1f, 1.0f, 0.0f);
			CVector3f n(1.0f, 0.1f, 0.0f);
			CPlane plane(n, p);

			// slice
			float bufN[64][8] = {0.0f};
			float bufA[64][8] = {0.0f};
			int bufNCount = 0;
			int bufACount = 0;
			Chop(plane, &(normalsAndVertices[0][0]), sizeof(normalsAndVertices)/8/sizeof(float)
				, bufN, bufA, bufNCount, bufACount);

			Assert::IsTrue(bufNCount == 6);
			Assert::IsTrue(bufACount == 3);

			// 1st triangle
			Assert::IsTrue(FEQ(bufN[0][0], 0.5));
			Assert::IsTrue(FEQ(bufN[0][1], -0.5));
			Assert::IsTrue(FEQ(bufN[0][2], 0.5));

			Assert::IsTrue(FEQ(bufN[1][0], 0.5));
			Assert::IsTrue(FEQ(bufN[1][1], 0.5));
			Assert::IsTrue(FEQ(bufN[1][2], 0.5));

			Assert::IsTrue(FEQ(bufN[2][0], 0.05));
			Assert::IsTrue(FEQ(bufN[2][1], -0.5));
			Assert::IsTrue(FEQ(bufN[2][2], 0.5));

			// 2nd triangle
			Assert::IsTrue(FEQ(bufN[3][0], 0.5));
			Assert::IsTrue(FEQ(bufN[3][1], 0.5));
			Assert::IsTrue(FEQ(bufN[3][2], 0.5));

			Assert::IsTrue(FEQ(bufN[4][0], 0.0));
			Assert::IsTrue(FEQ(bufN[4][1], 0.0));
			Assert::IsTrue(FEQ(bufN[4][2], 0.5));

			Assert::IsTrue(FEQ(bufN[5][0], 0.05));
			Assert::IsTrue(FEQ(bufN[5][1], -0.5));
			Assert::IsTrue(FEQ(bufN[5][2], 0.5));

			// 3rd triangle
			Assert::IsTrue(FEQ(bufA[0][0], -0.5));
			Assert::IsTrue(FEQ(bufA[0][1], -0.5));
			Assert::IsTrue(FEQ(bufA[0][2],  0.5));

			Assert::IsTrue(FEQ(bufA[1][0], 0.05));
			Assert::IsTrue(FEQ(bufA[1][1], -0.5));
			Assert::IsTrue(FEQ(bufA[1][2], 0.5));

			Assert::IsTrue(FEQ(bufA[2][0], 0.0));
			Assert::IsTrue(FEQ(bufA[2][1], 0.0));
			Assert::IsTrue(FEQ(bufA[2][2], 0.5));
		}

		TEST_METHOD(TestMethod2)
		{
			const float normalsAndVertices[3][8] =
			{
				{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			};

			CVector3f p(-1.0f, 0.0f, 0.0f);
			CVector3f n(0.0f, 1.0f, 0.0f);
			CPlane plane(n, p);

			// slice
			float bufN[64][8] = {0.0f};
			float bufA[64][8] = {0.0f};
			int bufNCount = 0;
			int bufACount = 0;
			Chop(plane, &(normalsAndVertices[0][0]), sizeof(normalsAndVertices)/8/sizeof(float)
				, bufN, bufA, bufNCount, bufACount);

			Assert::IsTrue(bufNCount == 3);
			Assert::IsTrue(bufACount == 6);
			
			// 1st triangle
			Assert::IsTrue(FEQ(bufN[0][0], 0.5));
			Assert::IsTrue(FEQ(bufN[0][1], 0.5));
			Assert::IsTrue(FEQ(bufN[0][2], 0.5));

			Assert::IsTrue(FEQ(bufN[1][0], 0.0));
			Assert::IsTrue(FEQ(bufN[1][1], 0.0));
			Assert::IsTrue(FEQ(bufN[1][2], 0.5));

			Assert::IsTrue(FEQ(bufN[2][0], 0.5));
			Assert::IsTrue(FEQ(bufN[2][1], 0.0));
			Assert::IsTrue(FEQ(bufN[2][2], 0.5));

			//// 2nd triangle
			Assert::IsTrue(FEQ(bufA[0][0], -0.5));
			Assert::IsTrue(FEQ(bufA[0][1], -0.5));
			Assert::IsTrue(FEQ(bufA[0][2], 0.5));

			Assert::IsTrue(FEQ(bufA[1][0], 0.5));
			Assert::IsTrue(FEQ(bufA[1][1], -0.5));
			Assert::IsTrue(FEQ(bufA[1][2], 0.5));

			Assert::IsTrue(FEQ(bufA[2][0], 0.0));
			Assert::IsTrue(FEQ(bufA[2][1], 0.0));
			Assert::IsTrue(FEQ(bufA[2][2], 0.5));

			//// 3rd triangle
			Assert::IsTrue(FEQ(bufA[3][0], 0.5));
			Assert::IsTrue(FEQ(bufA[3][1], -0.5));
			Assert::IsTrue(FEQ(bufA[3][2], 0.5));

			Assert::IsTrue(FEQ(bufA[4][0], 0.5));
			Assert::IsTrue(FEQ(bufA[4][1], 0.0));
			Assert::IsTrue(FEQ(bufA[4][2], 0.5));

			Assert::IsTrue(FEQ(bufA[5][0], 0.0));
			Assert::IsTrue(FEQ(bufA[5][1], 0.0));
			Assert::IsTrue(FEQ(bufA[5][2], 0.5));
		}

		TEST_METHOD(TestMethod3)
		{
			const float normalsAndVertices[3][8] =
			{
				{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			};

			CVector3f p(0.0f, -0.5f, 0.0f);
			CVector3f n((float)(-1.0/sqrt(2.0)), (float)(1.0/sqrt(2.0)), 0.0f);
			CPlane plane(n, p);

			// slice
			float bufN[64][8] = {0.0f};
			float bufA[64][8] = {0.0f};
			int bufNCount = 0;
			int bufACount = 0;
			Chop(plane, &(normalsAndVertices[0][0]), sizeof(normalsAndVertices)/8/sizeof(float)
				, bufN, bufA, bufNCount, bufACount);

			Assert::IsTrue(bufNCount == 6);
			Assert::IsTrue(bufACount == 3);

			// 1st triangle
			Assert::IsTrue(FEQ(bufN[0][0], 0.5));
			Assert::IsTrue(FEQ(bufN[0][1], 0.5));
			Assert::IsTrue(FEQ(bufN[0][2], 0.5));

			Assert::IsTrue(FEQ(bufN[1][0], -0.5));
			Assert::IsTrue(FEQ(bufN[1][1], -0.5));
			Assert::IsTrue(FEQ(bufN[1][2], 0.5));

			Assert::IsTrue(FEQ(bufN[2][0], 0.5));
			Assert::IsTrue(FEQ(bufN[2][1], 0.0));
			Assert::IsTrue(FEQ(bufN[2][2], 0.5));

			// 2nd triangle
			Assert::IsTrue(FEQ(bufN[3][0], -0.5));
			Assert::IsTrue(FEQ(bufN[3][1], -0.5));
			Assert::IsTrue(FEQ(bufN[3][2], 0.5));

			Assert::IsTrue(FEQ(bufN[4][0], 0.0));
			Assert::IsTrue(FEQ(bufN[4][1], -0.5));
			Assert::IsTrue(FEQ(bufN[4][2], 0.5));

			Assert::IsTrue(FEQ(bufN[5][0], 0.5));
			Assert::IsTrue(FEQ(bufN[5][1], 0.0));
			Assert::IsTrue(FEQ(bufN[5][2], 0.5));

			// 3rd triangle
			Assert::IsTrue(FEQ(bufA[0][0], 0.5));
			Assert::IsTrue(FEQ(bufA[0][1], -0.5));
			Assert::IsTrue(FEQ(bufA[0][2], 0.5));

			Assert::IsTrue(FEQ(bufA[1][0], 0.5));
			Assert::IsTrue(FEQ(bufA[1][1], 0.0));
			Assert::IsTrue(FEQ(bufA[1][2], 0.5));

			Assert::IsTrue(FEQ(bufA[2][0], 0.0));
			Assert::IsTrue(FEQ(bufA[2][1], -0.5));
			Assert::IsTrue(FEQ(bufA[2][2], 0.5));
		}

		TEST_METHOD(TestMethod4)
		{
			const float normalsAndVertices[3][8] =
			{
				{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			};

			CVector3f p(-0.5f, -0.5f, 0.0f);
			CVector3f n(0.0f, -1.0f, 0.0f);
			CPlane plane(n, p);

			// slice
			float bufN[64][8] = {0.0f};
			float bufA[64][8] = {0.0f};
			int bufNCount = 0;
			int bufACount = 0;
			Chop(plane, &(normalsAndVertices[0][0]), sizeof(normalsAndVertices)/8/sizeof(float)
				, bufN, bufA, bufNCount, bufACount);

			Assert::IsTrue(bufNCount == 0);
			Assert::IsTrue(bufACount == 3);

			// 1st triangle
			Assert::IsTrue(FEQ(bufA[0][0],-0.5));
			Assert::IsTrue(FEQ(bufA[0][1],-0.5));
			Assert::IsTrue(FEQ(bufA[0][2], 0.5));

			Assert::IsTrue(FEQ(bufA[1][0], 0.5));
			Assert::IsTrue(FEQ(bufA[1][1],-0.5));
			Assert::IsTrue(FEQ(bufA[1][2], 0.5));

			Assert::IsTrue(FEQ(bufA[2][0], 0.5));
			Assert::IsTrue(FEQ(bufA[2][1], 0.5));
			Assert::IsTrue(FEQ(bufA[2][2], 0.5));
		}

		TEST_METHOD(TestTriangulation)
		{
			const float normalsAndVertices[][8] =
			{
				/* ëO */
				{ -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
				{  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
				{  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},

				{  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
				{ -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
				{ -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},

				/* å„ */
				{  0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},
				{ -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},
				{ -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},

				{ -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},
				{  0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},
				{  0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},

				/* âE */
				{  0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},

				{  0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},

				/* ç∂ */
				{ -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},

				{ -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},

				/* è„ */
				{ -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},

				{  0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},

				/* â∫ */
				{  0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f},
				{ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f},

				{ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f},
				{  0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f}
			};

			int len = sizeof(normalsAndVertices)/6/sizeof(float);
			float bufN[64][8] = {0.0f};
			float bufA[64][8] = {0.0f};
			int bufNCount = 0;
			int bufACount = 0;

			CVector3f p(-0.1f, 1.0f, 0.0f);
			CVector3f n(1.0f, 0.1f, 0.0f);
			CPlane plane(n, p);
			Chop(plane, &(normalsAndVertices[0][0]), sizeof(normalsAndVertices)/8/sizeof(float)
				, bufN, bufA, bufNCount, bufACount);

			Assert::IsTrue(bufNCount == 60);
			Assert::IsTrue(bufACount == 60);
		}
	};
}