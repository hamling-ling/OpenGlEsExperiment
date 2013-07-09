#include "pch.h"
#include "Triangle3f.h"


CTriangle3f::CTriangle3f()
{
	MakeZero();
}


CTriangle3f::CTriangle3f(const CTriangle3f& tri)
{
	SetValue(tri.m_vecs);
}


CTriangle3f::CTriangle3f(const CVector3f vecs[VERTICES])
{
	SetValue(vecs);
}


CTriangle3f::CTriangle3f(CVector3f a, CVector3f b, CVector3f c)
{
	SetValue(a, b, c);
}


CTriangle3f::~CTriangle3f()
{
}


void
CTriangle3f::SetValue(const CVector3f vecs[VERTICES])
{
	SetValue(vecs[0], vecs[1], vecs[2]);
}


void
CTriangle3f::SetValue(CVector3f a, CVector3f b, CVector3f c)
{
	m_vecs[A] = a;
	m_vecs[B] = b;
	m_vecs[C] = c;
}


void
CTriangle3f::GetValue(CVector3f vecs[VERTICES]) const
{
	GetValue(vecs[A], vecs[B], vecs[C]);
}


void
CTriangle3f::GetValue(CVector3f& a, CVector3f& b, CVector3f& c) const
{
	a = m_vecs[A];
	b = m_vecs[B];
	c = m_vecs[C];
}


CTriangle3f&
CTriangle3f::operator=(const CTriangle3f& tri)
{
	SetValue(tri.m_vecs);
	return *this;
}


CVector3f&
CTriangle3f::operator[](int i)
{
	return m_vecs[i];
}


CVector3f
CTriangle3f::operator[](int i) const
{
	return m_vecs[i];
}


void
CTriangle3f::MakeZero()
{
	CVector3f zero(0.0f, 0.0f, 0.0f);
	SetValue(zero, zero, zero);
}


bool
operator==(const CTriangle3f& tri1, const CTriangle3f& tri2)
{
	bool equal;
	equal = (tri1[CTriangle3f::A] == tri2[CTriangle3f::A]) &&
	        (tri1[CTriangle3f::B] == tri2[CTriangle3f::B]) &&
	        (tri1[CTriangle3f::C] == tri2[CTriangle3f::C]);

	return equal;
}


bool
operator!=(const CTriangle3f& tri1, const CTriangle3f& tri2)
{
	bool notEqual;
	notEqual = (tri1[CTriangle3f::A] != tri2[CTriangle3f::A]) ||
	           (tri1[CTriangle3f::B] != tri2[CTriangle3f::B]) ||
	           (tri1[CTriangle3f::C] != tri2[CTriangle3f::C]);

	return notEqual;
}
