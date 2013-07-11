#include "stdafx.h"
#include "Vertex.h"



CVertex::CVertex()
{
	MakeZero();
}


CVertex::CVertex(const CVertex& vec)
{
	SetValue(vec.m_vec);
}


CVertex::CVertex(const float vec[DIMENSION])
{
	SetValue(vec);
}


CVertex::CVertex(const CVector3f& normal, const CVector3f &point)
{
	SetValue(normal, point);
}


CVertex::CVertex(float nx, float ny, float nz, float vx, float vy, float vz)
{
	SetValue(nx, ny, nz, vx , vy, vz);
}


CVertex::~CVertex()
{
}


void CVertex::SetValue(const float vec[DIMENSION])
{
	SetValue(vec[NX], vec[NY], vec[NZ], vec[VX], vec[VY], vec[VZ]);
}


void CVertex::SetValue(const CVector3f& normal, const CVector3f &point)
{
	SetValue(
		normal[CVector3f::X],
		normal[CVector3f::X],
		normal[CVector3f::X],
		point[CVector3f::X],
		point[CVector3f::X],
		point[CVector3f::X]);
}


void CVertex::SetValue(float nx, float ny, float nz, float vx, float vy, float vz)
{
	m_vec[NX] = nx;
	m_vec[NY] = ny;
	m_vec[NZ] = nz;
	m_vec[VX] = vx;
	m_vec[VY] = vy;
	m_vec[VZ] = vz;
}


void CVertex::GetValue(float vec[DIMENSION]) const
{
	GetValue(vec[NX], vec[NY], vec[NZ], vec[VX], vec[VY], vec[VZ]);
}


void CVertex::GetValue(CVector3f& normal, CVector3f &point) const
{
	normal.SetValue(m_vec[CVertex::NX], m_vec[CVertex::NY], m_vec[CVertex::NZ]);
	point.SetValue(m_vec[CVertex::VX], m_vec[CVertex::VY], m_vec[CVertex::VZ]);
}


void CVertex::GetValue(float &nx, float &ny, float &nz, float &vx, float &vy, float &vz) const
{
	nx = m_vec[NX];
	ny = m_vec[NY];
	nz = m_vec[NZ];
	vx = m_vec[VX];
	ny = m_vec[VY];
	nz = m_vec[VZ];
}


CVector3f CVertex::GetNormal()
{
	return CVector3f(m_vec);
}


CVector3f CVertex::GetPoint()
{
	return CVector3f(&(m_vec[3]));
}


CVertex& CVertex::operator=(const CVertex& vec)
{
	SetValue(vec.m_vec);
	return *this;
}


float& CVertex::operator[](int i)
{
	return m_vec[i];
}


float CVector3f::operator[](int i) const
{
	return m_vec[i];
}


void CVertex::MakeZero()
{
	SetValue(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}


bool operator==(const CVertex& vec1, const CVertex& vec2)
{
	bool equal;
	equal = (vec1.m_vec[CVector3f::X] == vec2.m_vec[CVector3f::X]) &&
	        (vec1.m_vec[CVector3f::Y] == vec2.m_vec[CVector3f::Y]) &&
	        (vec1.m_vec[CVector3f::Z] == vec2.m_vec[CVector3f::Z]);

	return equal;
}


bool operator!=(const CVertex& vec1, const CVertex& vec2)
{
	bool notEqual;
	notEqual = (vec1.m_vec[CVector3f::X] != vec2.m_vec[CVector3f::X]) ||
	           (vec1.m_vec[CVector3f::Y] != vec2.m_vec[CVector3f::Y]) ||
	           (vec1.m_vec[CVector3f::Z] != vec2.m_vec[CVector3f::Z]);

	return notEqual;
}
