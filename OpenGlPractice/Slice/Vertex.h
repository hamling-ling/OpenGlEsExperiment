#pragma once

#include "Vector3f.h"

class CVertex
{
public:
	enum {
		NX = 0,
		NY = 1,
		NZ = 2,
		VX = 3,
		VY = 4,
		VZ = 5,
		DIMENSION = 6
	};

private:
	float m_vec[DIMENSION];

public:
	CVertex();
	CVertex(const CVertex& vec);
	CVertex(const float vec[DIMENSION]);
	CVertex(const CVector3f& normal, const CVector3f &point);
	CVertex(float nx, float ny, float nz, float vx, float vy, float vz);
	virtual ~CVertex();

	void SetValue(const float vec[DIMENSION]);
	void SetValue(const CVector3f& normal, const CVector3f &point);
	void SetValue(float nx, float ny, float nz, float vx, float vy, float vz);
	void SetNormal(const CVector3f& n);

	void GetValue(float vec[DIMENSION]) const;
	void GetValue(CVector3f& normal, CVector3f &point) const;
	void GetValue(float &nx, float &ny, float &nz, float &vx, float &vy, float &vz) const;
	CVector3f GetNormal() const;
	CVector3f GetPoint() const;

	float& operator[](int i);
	float operator[](int i) const;

	CVertex& operator=(const CVertex& vec);

	float Length() const;

	void MakeZero();

	void Negate();

	float Normalize();

	friend bool operator==(const CVertex& vec1, const CVertex& vec2);
	friend bool operator!=(const CVertex& vec1, const CVertex& vec2);
};


