#ifndef __MODELTOOL_H__
#define __MODELTOOL_H__

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdarg>
#include <vector>
#include <algorithm>
#include "ModelType.h"

#define GRAVITY_CONST		9.8f
#define BIG_NUMBER			((MODELFLOAT)1e5)
#define SMALL_NUMBER		((MODELFLOAT)1e-5)
#define IS_POSITIVE(x)		(x >= 0.0)
#define SIGN(x)				((x)<0.0?-1.0:1.0)
#define IS_ALMOST_ZERO(x)	(fabs(x) < SMALL_NUMBER)
#define ZEROMEMORY(x)		memset(&x, 0, sizeof(x))
#define SAFE_DELETE(p)		{ delete p; p=NULL; }

#define MODEL_INLINE static inline

using namespace std;

namespace osakanaengine {

	static const MODELFLOAT TWO_M_PI = static_cast<MODELFLOAT>(2.0 * M_PI);
	static const MODELVEC3D G = { 0.0f, -GRAVITY_CONST, 0.0f};
	static const MODELVEC3D XAXIS = {1.0, 0.0, 0.0};
	static const MODELVEC3D YAXIS = {0.0, 1.0, 0.0};
	static const MODELVEC3D ZAXIS = {0.0, 0.0, 1.0};
    static const MODELVEC3D ZEROVEC3D = {0.0, 0.0, 0.0};
	static const ModelPoint ZEROPOINT = {0.0, 0.0};
	
    MODEL_INLINE ModelPoint MODELPOINTMake(MODELFLOAT x, MODELFLOAT y);
	MODEL_INLINE MODELVEC3D MODELVEC3DMake(MODELFLOAT x, MODELFLOAT y, MODELFLOAT z);
	MODEL_INLINE MODELLINE MODELLINEMake(MODELVEC3D a, MODELVEC3D b);
	MODEL_INLINE MODELVERTEX MODELVERTEXMake(MODELVEC3D p, MODELVEC3D n, ModelPoint t);
	MODEL_INLINE MODELVEC3D MODELVEC3DMakeWithArray(MODELFLOAT array[3]);
	MODEL_INLINE MODELVERTEX MODELVERTEXMakeWithArray(MODELFLOAT array[8]);
	MODEL_INLINE ModelPoint AddPoint(const ModelPoint a, const ModelPoint b);
    MODEL_INLINE ModelPoint SubtPoint(const ModelPoint a, const ModelPoint b);
	MODEL_INLINE ModelPoint ScalePoint(const MODELFLOAT f, const ModelPoint v);
    MODEL_INLINE MODELFLOAT AngleBetween(const ModelPoint a, const ModelPoint b);
    MODEL_INLINE MODELFLOAT DotProduct(const ModelPoint a, const ModelPoint b);
    MODEL_INLINE MODELFLOAT CrossProduct(const ModelPoint a, const ModelPoint b);
    
	MODEL_INLINE MODELVEC3D NormalForce(const MODELFLOAT mass, const MODELVEC3D normal);
	MODEL_INLINE MODELVEC3D ProjBOntoA(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE MODELFLOAT Magnitude(const MODELVEC3D a);
	MODEL_INLINE MODELFLOAT DotProduct(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE MODELVEC3D CrossProduct(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE MODELVEC3D ScaleVec(const MODELFLOAT f, const MODELVEC3D v);
	MODEL_INLINE MODELVEC3D AddVec(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE MODELVEC3D SubtVec(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE MODELVEC3D RotateY(const MODELVEC3D src, const MODELFLOAT angle);
	MODEL_INLINE MODELVEC3D FreeRotate(const MODELVEC3D src, const MODELVEC3D axis, const MODELFLOAT angle, MODELVEC3D ret);
	MODEL_INLINE MODELVEC3D Normalize(const MODELVEC3D src);
	MODEL_INLINE MODELFLOAT SubtFriction(const MODELFLOAT force, const MODELFLOAT friction);
	MODEL_INLINE MODELVEC3D SubtFriction(const MODELVEC3D force, const MODELVEC3D friction);
	MODEL_INLINE MODELVEC3D SubtFriction(const MODELVEC3D force, const MODELFLOAT friction);
	MODEL_INLINE MODELFLOAT GripTurn(const MODELFLOAT centripetal, const MODELFLOAT grip);
	MODEL_INLINE MODELVEC3D GripTurn(const MODELVEC3D centripetal, const MODELVEC3D grip, MODELVEC3D ret);
	MODEL_INLINE MODELFLOAT AngleBetween(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE MODELVEC3D EulerMethod(const MODELVEC3D Rhs, const MODELVEC3D Xcur, const MODELFLOAT dt, MODELVEC3D Xnext);
	MODEL_INLINE MODELVEC3D GetTorque(const MODELVEC3D R, const MODELFLOAT r, const MODELVEC3D F);
	MODEL_INLINE MODELVEC3D MatTrans(const MODELVEC3D X, const MODELFLOAT MAT[3][3]);
	MODEL_INLINE MODELVEC3D RotateAroundX(const MODELVEC3D X, const MODELFLOAT theta);
	MODEL_INLINE MODELVEC3D RotateAroundY(const MODELVEC3D X, const MODELFLOAT theta);
	MODEL_INLINE MODELVEC3D RotateAroundZ(const MODELVEC3D X, const MODELFLOAT theta);
    MODEL_INLINE MODELVEC3D RotateAroundXYZ(const MODELVEC3D X, const MODELVEC3D angle);
	MODEL_INLINE MODELVEC3D AngleToDirY(const MODELVEC3D Angle);
	MODEL_INLINE MODELVEC3D AddMultiVec(const int num, ...);
	MODEL_INLINE MODELVEC3D SubtMultiVec(const int num, ...);
	MODEL_INLINE MODELFLOAT NormalizeAngle(const MODELFLOAT original);
	MODEL_INLINE bool IsInsideBox(const MODELVEC3D pos, const MODELVEC3D rect[4]);
	MODEL_INLINE MODELFLOAT Distance(const MODELVEC3D a, const MODELVEC3D b);
	MODEL_INLINE bool GetIntersection(const MODELVEC3D a, const MODELVEC3D b, const MODELVEC3D c, const MODELVEC3D d, MODELVEC3D* ret);
	MODEL_INLINE void ApplyReflection(const MODELVEC3D prevPos, const MODELVEC3D nextPos, const MODELVEC3D is, const MODELVEC3D a, const MODELVEC3D b, MODELVEC3D *vel);
	MODEL_INLINE MODELLINE RotateLine(const MODELFLOAT angle, const MODELVEC3D center, MODELLINE line);
	MODEL_INLINE MODELRECT RotateRect(const MODELFLOAT angle, const MODELVEC3D center);

    template<class T>
    void DeletePointerVectors(std::vector<T> &vec);

    template<class T>
    bool RemoveFromVector(std::vector<T> &vec, T value);
    
    bool StringEndsWith(std::string const &fullString, std::string const &ending);
	
    MODEL_INLINE ModelPoint MODELPOINTMake(MODELFLOAT x, MODELFLOAT y)
    {
        ModelPoint p = {x,y};
        return p;
    }
    
	MODEL_INLINE MODELVEC3D MODELVEC3DMake(MODELFLOAT x, MODELFLOAT y, MODELFLOAT z)
	{
		MODELVEC3D vec3d = { x, y, z};
		return vec3d;
	}
	
	MODEL_INLINE MODELVEC3D MODELVEC3DMakeWithArray(MODELFLOAT array[3])
	{
		MODELVEC3D vec3d = { array[0], array[1], array[2]};
		return vec3d;
	}
	
	MODEL_INLINE MODELLINE MODELLINEMake(MODELVEC3D a, MODELVEC3D b)
	{
		MODELLINE line = {a,b};
		return line;
	}

	MODEL_INLINE MODELVERTEX MODELVERTEXMake(MODELVEC3D p, MODELVEC3D n, ModelPoint t)
	{
		MODELVERTEX vert = {p,n,t};
		return vert;
	}

	MODEL_INLINE MODELVERTEX MODELVERTEXMakeWithArray(MODELFLOAT array[8])
	{
		MODELVEC3D p = { array[0], array[1], array[2]};
		MODELVEC3D n = { array[3], array[4], array[5]};
		ModelPoint t = { array[6], array[7]};
	
		MODELVERTEX vert = {p,n,t};
		return vert;
	}

	MODEL_INLINE MODELTRIANGLE MODELTRIANGLEMake(MODELVERTEX a, MODELVERTEX b, MODELVERTEX c)
	{
		MODELTRIANGLE tri = {a,b,c};
		return tri;
	}

	MODEL_INLINE ModelPoint AddPoint(const ModelPoint a, const ModelPoint b)
    {
        ModelPoint c = { a.x + b.x, a.y + b.y};
        return c;
    }
	
    MODEL_INLINE ModelPoint SubtPoint(const ModelPoint a, const ModelPoint b)
    {
        ModelPoint c = { a.x - b.x, a.y - b.y};
        return c;
    }
    
	MODEL_INLINE ModelPoint ScalePoint(const MODELFLOAT f, const ModelPoint v)
	{
		ModelPoint vec = {
			f * v.x,
			f * v.y,
		};
		return vec;
	}
	
    MODEL_INLINE MODELFLOAT AngleBetween(const ModelPoint a, const ModelPoint b)
    {
        MODELFLOAT dot = DotProduct(a, b);
        MODELFLOAT cross = CrossProduct(a, b);
        
        return atan2f(cross, dot);
    }
    
    MODEL_INLINE MODELFLOAT DotProduct(const ModelPoint a, const ModelPoint b)
    {
        return a.x * b.x + a.y * b.y;
    }
    
    MODEL_INLINE MODELFLOAT CrossProduct(const ModelPoint a, const ModelPoint b)
    {
        return a.x * b.y - a.y * b.x;
    }
	
	MODEL_INLINE MODELVEC3D NormalForce(const MODELFLOAT mass, const MODELVEC3D normal)
	{
		MODELVEC3D mGup = ScaleVec(-mass, G);
		return ProjBOntoA( normal, mGup);
	}
	
	MODEL_INLINE MODELVEC3D ProjBOntoA(const MODELVEC3D a, const MODELVEC3D b)
	{
		// projection of B onto A : projA_B = (B dot A)/|A|^2 * A
		MODELFLOAT ab = DotProduct(a, b);
		if(ab == 0.0)
		{
			return b;
		}

		MODELFLOAT aa = DotProduct(a, a);
		return ScaleVec( ab/aa, a);
	}
	
	MODEL_INLINE MODELFLOAT Magnitude(const MODELVEC3D a)
	{
		return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
	}
	
	MODEL_INLINE MODELFLOAT DotProduct(const MODELVEC3D a, const MODELVEC3D b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	
	MODEL_INLINE MODELVEC3D CrossProduct(const MODELVEC3D a, const MODELVEC3D b)
	{
		MODELVEC3D vec = {
			a.y * b.z - b.y * a.z,//y1*z2 - y2*z1;
			a.z * b.x - b.z * a.x,//z1*x2 - z2*x1;
			a.x * b.y - b.x * a.y,//x1*y2 - x2*y1;
		};
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D ScaleVec(const MODELFLOAT f, const MODELVEC3D v)
	{
		MODELVEC3D vec = {
			f * v.x,
			f * v.y,
			f * v.z,
		};
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D AddVec(const MODELVEC3D a, const MODELVEC3D b)
	{
		MODELVEC3D vec = {
			a.x + b.x,
			a.y + b.y,
			a.z + b.z,
		};
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D AddMultiVec(const int num, ...)
	{
		MODELVEC3D tot = {0.0};
		
		va_list ap;
		va_start(ap, num);
		for( int i=0; i < num; i++)
		{
			MODELVEC3D p = va_arg(ap, MODELVEC3D);
			tot = AddVec(tot, p);
		}
		
		return tot;
	}
	
	MODEL_INLINE MODELVEC3D SubtMultiVec(const int num, ...)
	{
		if(num == 0)
			return ZEROVEC3D;
		
		MODELVEC3D tot = {0.0};
		va_list ap;
		va_start(ap, num);
		
		MODELVEC3D p = va_arg(ap, MODELVEC3D);
		tot = p;
		
		for( int i=1; i < num; i++)
		{
			p = va_arg(ap, MODELVEC3D);
			tot = SubtVec(tot, p);
		}
		
		return tot;
	}
	
	MODEL_INLINE MODELVEC3D SubtVec(const MODELVEC3D a, const MODELVEC3D b)
	{
		MODELVEC3D vec = {
			a.x - b.x,
			a.y - b.y,
			a.z - b.z,
		};
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D RotateY(const MODELVEC3D src, const MODELFLOAT angle)
	{
		MODELFLOAT c = cos(angle);
		MODELFLOAT s = sin(angle);
		
		MODELFLOAT m00 = c;
		MODELFLOAT m01 = 0.0;
		MODELFLOAT m02 = s;
		
		MODELFLOAT m10 = 0.0;
		MODELFLOAT m11 = 1.0;
		MODELFLOAT m12 = 0.0;
		
		MODELFLOAT m20 = -s;
		MODELFLOAT m21 = 0.0;
		MODELFLOAT m22 = c;
		
		MODELVEC3D vec = {
			m00 * src.x + m01 * src.y + m02 * src.z,
			m10 * src.x + m11 * src.y + m12 * src.z,
			m20 * src.x + m21 * src.y + m22 * src.z,
		};
		
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D FreeRotate(const MODELVEC3D src, const MODELVEC3D axis, const MODELFLOAT angle)
	{
		// get rotation matrix
		MODELFLOAT cosine = cos(angle);
		MODELFLOAT one_sub_cos = 1 - cosine;
		MODELFLOAT sine = sin(angle);
		MODELFLOAT x = axis.x;
		MODELFLOAT y = axis.y;
		MODELFLOAT z = axis.z;
		MODELFLOAT xx = pow(x,2);
		MODELFLOAT xy = x * y;
		MODELFLOAT xz = x * z;
		MODELFLOAT yy = pow(y,2);
		MODELFLOAT yz = y * z;
		MODELFLOAT zz = pow(z,2);
		MODELFLOAT xsine = x * sine;
		MODELFLOAT ysine = y * sine;
		MODELFLOAT zsine = z * sine;
		
		MODELFLOAT m00 = xx * one_sub_cos + cosine;
		MODELFLOAT m01 = xy * one_sub_cos - zsine;
		MODELFLOAT m02 = xz * one_sub_cos + ysine;
		
		MODELFLOAT m10 = xy * one_sub_cos + zsine;
		MODELFLOAT m11 = yy * one_sub_cos + cosine;
		MODELFLOAT m12 = yz * one_sub_cos - xsine;
		
		MODELFLOAT m20 = xz * one_sub_cos - ysine;
		MODELFLOAT m21 = yz * one_sub_cos + xsine;
		MODELFLOAT m22 = zz * one_sub_cos + cosine;
		
		MODELVEC3D vec = {
			m00 * src.x + m01 * src.y + m02 * src.z,
			m10 * src.x + m11 * src.y + m12 * src.z,
			m20 * src.x + m21 * src.y + m22 * src.z,
		};
		
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D Normalize(const MODELVEC3D src)
	{
		MODELFLOAT mag = Magnitude(src);

		MODELVEC3D vec = {
			src.x / mag,
			src.y / mag,
			src.z / mag,
		};
		
		return vec;
	}
	
	MODEL_INLINE MODELFLOAT SubtFriction(const MODELFLOAT force, const MODELFLOAT friction)
	{
		bool wasPositive = true;
		MODELFLOAT subt = 0.0;
		
		wasPositive = IS_POSITIVE(force);
		subt = force - friction;
		if(IS_POSITIVE(subt) == wasPositive)
			return subt;
		
		return (MODELFLOAT)0.0;
	}
	
	MODEL_INLINE MODELVEC3D SubtFriction(const MODELVEC3D force, const MODELVEC3D friction)
	{
		MODELVEC3D vec = {
			SubtFriction(force.x, friction.x),
			SubtFriction(force.y, friction.y),
			SubtFriction(force.z, friction.z),
		};
		
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D SubtFriction(const MODELVEC3D force, const MODELFLOAT friction)
	{
		MODELFLOAT fmag = Magnitude(force);
		if(fmag < friction)
		{
			return force;
		}
		
		MODELFLOAT rate = (fmag-friction) / fmag;
		return ScaleVec(rate, force);
	}
	
	MODEL_INLINE MODELFLOAT GripTurn(const MODELFLOAT centripetal, const MODELFLOAT grip)
	{
		if(fabs(centripetal) <= fabs(grip))
			return centripetal;
		
		return grip - centripetal;
	}
	
	/**
	 * centripetal and friction must be same direction
	 */
	MODEL_INLINE MODELVEC3D GripTurn(const MODELVEC3D centripetal, const MODELVEC3D grip, MODELVEC3D *ret)
	{
		MODELVEC3D vec = {
			GripTurn(centripetal.x, grip.x),
			GripTurn(centripetal.y, grip.y),
			GripTurn(centripetal.z, grip.z),
		};
		
		return vec;
	}
	
	MODEL_INLINE MODELFLOAT AngleBetween(const MODELVEC3D a, const MODELVEC3D b)
	{
		MODELFLOAT a_dot_b = DotProduct(a, b);
		MODELFLOAT magA = Magnitude(a);
		MODELFLOAT magB = Magnitude(b);
		MODELFLOAT denom = magA * magB;
		if(denom == 0.0)
			return 0.0;
		
		return acos(a_dot_b / denom);
	}
	
	MODEL_INLINE MODELVEC3D EulerMethod(const MODELVEC3D Rhs, const MODELVEC3D Xcur, const MODELFLOAT dt)
	{
		MODELVEC3D vec = {
			Rhs.x * dt + Xcur.x,
			Rhs.y * dt + Xcur.y,
			Rhs.z * dt + Xcur.z,
		};
		
		return vec;
	}
	
	/**
	 * R is unit vector
	 */
	MODEL_INLINE MODELVEC3D GetTorque(const MODELVEC3D R, const MODELFLOAT r, const MODELVEC3D F)
	{
		MODELVEC3D Rvec = ScaleVec(r, R);
		return CrossProduct(Rvec, F);
	}
	
	MODEL_INLINE MODELVEC3D MatTrans(const MODELVEC3D X, const MODELFLOAT MAT[3][3])
	{
		MODELVEC3D vec = {
			X.x * MAT[0][0] + X.y * MAT[0][1] + X.z * MAT[0][2],
			X.x * MAT[1][0] + X.y * MAT[1][1] + X.z * MAT[1][2],
			X.x * MAT[2][0] + X.y * MAT[2][1] + X.z * MAT[2][2],
		};
		
		return vec;
	}
	
	MODEL_INLINE MODELVEC3D RotateAroundX(const MODELVEC3D X, const MODELFLOAT theta)
	{
		MODELFLOAT cosine = cos(theta);
		MODELFLOAT sine = sin(theta);
		MODELFLOAT MAT[3][3] = {
			{1.0,	0.0,		0.0},
			{0.0,	cosine,		-sine},
			{0.0,	sine,		cosine}
		};
		
		return MatTrans(X, MAT);
	}
	
	MODEL_INLINE MODELVEC3D RotateAroundY(const MODELVEC3D X, const MODELFLOAT theta)
	{
		MODELFLOAT cosine = cos(theta);
		MODELFLOAT sine = sin(theta);
		MODELFLOAT MAT[3][3] = {
			{cosine,	0.0,	sine},
			{0.0,		1.0,	0.0},
			{-sine,		0.0,	cosine}
		};
		
		return MatTrans(X, MAT);
	}
	
	MODEL_INLINE MODELVEC3D RotateAroundZ(const MODELVEC3D X, const MODELFLOAT theta)
	{
		MODELFLOAT cosine = cos(theta);
		MODELFLOAT sine = sin(theta);
		MODELFLOAT MAT[3][3] = {
			{cosine,	-sine,		0.0},
			{sine,		cosine,		0.0},
			{0.0,		0.0,		1.0}
		};
		
		return MatTrans(X, MAT);
	}
	
    MODEL_INLINE MODELVEC3D RotateAroundXYZ(const MODELVEC3D X, const MODELVEC3D angle)
    {
        MODELVEC3D rotX = RotateAroundX(X,     angle.x);
        MODELVEC3D rotY = RotateAroundY(rotX, angle.y);
        MODELVEC3D rotZ = RotateAroundZ(rotY, angle.z);
		
        return rotZ;
    }
	
	MODEL_INLINE MODELVEC3D AngleToDirY(const MODELVEC3D Angle)
	{
		return RotateAroundY(ZAXIS, Angle.y);
	}
	
	MODEL_INLINE MODELFLOAT NormalizeAngle(const MODELFLOAT original)
	{
		MODELFLOAT a = fmod(original, TWO_M_PI);
		
		MODELFLOAT pinum = (MODELFLOAT)(int)(a / M_PI);
		MODELFLOAT ans = (-TWO_M_PI) * pinum + a;
		
		return ans;
	}
	
	MODEL_INLINE MODELVEC3D NormalizeAngle(MODELVEC3D angle)
	{
		MODELVEC3D vec = {
			NormalizeAngle(angle.x),
			NormalizeAngle(angle.y),
			NormalizeAngle(angle.z),
		};
		
		return vec;
	}
	
	MODEL_INLINE bool IsInsideBox(const MODELVEC3D pos, const MODELVEC3D box[4])
	{
		int numPositive = 0;
		
		for(int i = 0; i < 4; i++)
		{
			MODELVEC3D ab = SubtVec(box[(i+1)%4], box[i]);
			MODELVEC3D ap = SubtVec(pos, box[i]);
			
			MODELFLOAT ab_ap = ab.x * ap.z - ap.x * ab.z;
			if(IS_POSITIVE(ab_ap))
				numPositive++;
		}
		
		return (numPositive == 0 || numPositive == 4);
	}
	
	MODEL_INLINE MODELFLOAT Distance(const MODELVEC3D a, const MODELVEC3D b)
	{
		MODELVEC3D c = SubtVec( b, a);
		return sqrt(pow(c.x, 2)+pow(c.y,2)+pow(c.z,2));
	}
	
	MODEL_INLINE bool GetIntersection(const MODELVEC3D a, const MODELVEC3D b, const MODELVEC3D c, const MODELVEC3D d, MODELVEC3D* ret)
	{
		MODELFLOAT bxmax = b.x - a.x;
		MODELFLOAT dxmcx = d.x - c.x;
		MODELVEC3D cand;
		if(!IS_ALMOST_ZERO(bxmax) && !IS_ALMOST_ZERO(dxmcx))
		{
			// Ax-y+C=0
			MODELFLOAT A = (b.z - a.z)/bxmax;
			MODELFLOAT C = (a.z * b.x - a.x * b.z)/bxmax;
			
			// Dx-y+F=0
			MODELFLOAT D = (d.z - c.z)/dxmcx;
			MODELFLOAT F = (c.z * d.x - c.x * d.z)/dxmcx;
			
			if(A-D == 0.0)
				return false;
			
			cand.x = (F-C)/(A-D);
			cand.y = 0.0;
			cand.z = cand.x * A + C;
		}
		else if(IS_ALMOST_ZERO(bxmax) && !IS_ALMOST_ZERO(dxmcx))
		{
			// line A is perpendicular to X axis
			MODELFLOAT x1 = b.x;
			
			// Dx-y+F=0
			MODELFLOAT D = (d.z - c.z)/dxmcx;
			MODELFLOAT F = (c.z * d.x - c.x * d.z)/dxmcx;
			
			cand.x = x1;
			cand.y = 0.0;
			cand.z = cand.x * D + F;
		}
		else if(!IS_ALMOST_ZERO(bxmax) && IS_ALMOST_ZERO(dxmcx))
		{
			MODELFLOAT x2 = c.x;
			
			// Ax-y+C=0
			MODELFLOAT A = (b.z - a.z)/bxmax;
			MODELFLOAT C = (a.z * b.x - a.x * b.z)/bxmax;
			
			cand.x = x2;
			cand.y = 0.0;
			cand.z = cand.x * A + C;
		}
		else
		{
			return false;
		}
		
		if(std::min(a.x ,b.x) <= cand.x && cand.x <= std::max(a.x, b.x))
		{
			if(std::min(a.z, b.z) <= cand.z && cand.z <= std::max(a.z, b.z))
			{
				*ret = cand;
				return true;
			}
		}
		
		return false;
	}
	
	MODEL_INLINE void ApplyReflection(const MODELVEC3D prevPos, const MODELVEC3D nextPos, const MODELVEC3D is, const MODELVEC3D a, const MODELVEC3D b, MODELVEC3D *vel)
	{
        MODELVEC3D posVec = SubtVec(nextPos, prevPos);
        
        MODELVEC3D ab = SubtVec(a, b);
        
        MODELVEC3D prj = ProjBOntoA(ab, posVec);
        
        MODELVEC3D xp = CrossProduct(prj, posVec);
        
        // decompose vec into posVec direction and something else
        MODELVEC3D veloIntoPosVec = ProjBOntoA(posVec, *vel);
        MODELVEC3D veloIntoTheOther = SubtVec(*vel, veloIntoPosVec);

        // if posVec is left of prj, rotate negative angle
        MODELFLOAT angle = static_cast<MODELFLOAT>(-2.0f*SIGN(xp.y));
        MODELVEC3D veloIntoPosVecRef = RotateY(veloIntoPosVec, angle);
        
        *vel = AddVec(veloIntoPosVecRef, veloIntoTheOther);
	}
	
	MODEL_INLINE MODELLINE RotateLine(const MODELFLOAT angle, const MODELVEC3D center, MODELLINE line)
	{
		MODELLINE rotated;
		for(int i = 0; i < 2; i++)
		{
			MODELVEC3D rel_vec = SubtVec(line.v[i], center);
			
			MODELVEC3D rel_rot = RotateAroundY(rel_vec, angle);
			rotated.v[i] = AddVec(rel_rot, center);
		}
		return rotated;
	}
	
	MODEL_INLINE MODELRECT RotateRect(const MODELFLOAT angle, const MODELVEC3D center, MODELRECT rect)
	{
		MODELRECT rotated;
		for(int i = 0; i < 4; i++)
		{
			MODELVEC3D rel_vec = SubtVec(rect.v[i], center);
			
			MODELVEC3D rel_rot = RotateAroundY(rel_vec, angle);
			rotated.v[i] = AddVec(rel_rot, center);
		}
		return rotated;
	}
	
    template<class T>
    void DeletePointerVectors(std::vector<T> &vec)
    {
        while (!vec.empty()) {
            T p = vec.back();
            vec.pop_back();
            delete p;
        }
    }
	
    template<class T>
    bool RemoveFromVector(std::vector<T> &vec, T value)
    {
        typename std::vector<T>::iterator it = vec.end();
        
        it = std::find(vec.begin(), vec.end(), value);
        if(it != vec.end()) {
            vec.erase(it);
            return true;
        }
		
        return false;
    }
}

#endif
