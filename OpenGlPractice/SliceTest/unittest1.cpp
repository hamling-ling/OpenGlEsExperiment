#include "stdafx.h"
#include "CppUnitTest.h"

#include "Slice.h"
#include "Plane.h"

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
	};
}