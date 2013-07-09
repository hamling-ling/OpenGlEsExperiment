#ifndef _TRIANGLE3F_H_
#define _TRIANGLE3F_H_

#include "Vector3f.h"


class CTriangle3f
{
public:
	enum {
		A = 0,
		B = 1,
		C = 2,
		VERTICES = 3
	};

private:
	CVector3f m_vecs[VERTICES];

public:
	CTriangle3f();
	CTriangle3f(const CTriangle3f& vec);
	CTriangle3f(const CVector3f vecs[VERTICES]);
	CTriangle3f(CVector3f x, CVector3f y, CVector3f z);
	virtual ~CTriangle3f();

	void SetValue(const CVector3f vecs[VERTICES]);
	void SetValue(CVector3f a, CVector3f b, CVector3f c);

	void GetValue(CVector3f vecs[VERTICES]) const;
	void GetValue(CVector3f& a, CVector3f& b, CVector3f& c) const;

	CVector3f& operator[](int i);
	CVector3f operator[](int i) const;

	CTriangle3f& operator=(const CTriangle3f& tri);

	void MakeZero();

	friend bool operator==(const CTriangle3f& vec1, const CTriangle3f& vec2);
	friend bool operator!=(const CTriangle3f& vec1, const CTriangle3f& vec2);
};


#endif // _TRIANGLE3F_H_
