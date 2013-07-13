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

			// 1st triangle
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::X],-0.5));
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::Y],-0.5));
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::Z], 0.5));

			//Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::X],0.05));
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::Y],-0.5));
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::Z], 0.0));

			//Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::X], 0.0));
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::Y], 0.0));
			//Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::Z], 0.0));

			//// 2nd triangle
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::X], 0.5));
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::Y],-0.5));
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::Z], 0.5));

			//Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::X], 0.5));
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::Y], 0.5));
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::Z], 0.5));

			//Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::X], 0.05));
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::Y], -0.5));
			//Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::Z], 0.0));

			//// 3rd triangle
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::X], 0.5));
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::Y], 0.5));
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::Z], 0.5));

			//Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::X], 0.0));
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::Y], 0.0));
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::Z], 0.0));

			//Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::X], 0.05));
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::Y], -0.5));
			//Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::Z], 0.0));

		}

		TEST_METHOD(TestMethod2)
		{
			const float Vertices[3][3] =
			{
				{ -0.5f, -0.5f,  0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
			};

			CVector3f a(Vertices[0]);
			CVector3f b(Vertices[1]);
			CVector3f c(Vertices[2]);
			CTriangle tri0(a,b,c);

			CVector3f s0(-1.0f, 0.0f, 0.0f);
			CVector3f s1(1.0f, 0.0f, 0.0f);
			CTriangle sliced[3];

			bool isSliced = SliceTriangle(tri0, s0, s1, sliced);
			Assert::IsTrue(isSliced);

			// 1st triangle
			Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::Y], 0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::X], 0.0));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::Z], 0.0));

			Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::Z], 0.0));

			// 2nd triangle
			Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::X], -0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::Y],-0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::X], 0.0));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::Z], 0.0));

			// 3rd triangle
			Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::Z], 0.0));

			Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::X], 0.0));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::Z], 0.0));
		}

		TEST_METHOD(TestMethod3)
		{
			const float Vertices[3][3] =
			{
				{ -0.5f, -0.5f,  0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
			};

			CVector3f a(Vertices[0]);
			CVector3f b(Vertices[1]);
			CVector3f c(Vertices[2]);
			CTriangle tri0(a,b,c);

			CVector3f s0(0.0f, -0.5f, 0.0f);
			CVector3f s1(0.5f, 0.0f, 0.0f);
			CTriangle sliced[3];

			bool isSliced = SliceTriangle(tri0, s0, s1, sliced);
			Assert::IsTrue(isSliced);

			// 1st triangle
			Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::A][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::B][CVector3f::Z], 0.0));

			Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::X], 0.0));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[0][CTriangle::C][CVector3f::Z], 0.0));

			// 2nd triangle
			Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::Y], 0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::A][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::X], -0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::B][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[1][CTriangle::C][CVector3f::Z], 0.0));

			// 3rd triangle
			Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::X], -0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::A][CVector3f::Z], 0.5));

			Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::X], 0.0));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::Y], -0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::B][CVector3f::Z], 0.0));

			Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::X], 0.5));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::Y], 0.0));
			Assert::IsTrue(FEQ(sliced[2][CTriangle::C][CVector3f::Z], 0.0));
		}
	};
}