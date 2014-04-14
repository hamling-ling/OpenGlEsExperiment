#ifndef __MODELTYPE_H__
#define __MODELTYPE_H__

#define MODEL_DELTA_T		0.033

namespace osakanaengine {

	typedef float MODELFLOAT;

	typedef struct _MODELPOINT
	{
		struct { MODELFLOAT x, y; };
		MODELFLOAT v[2];
	} ModelPoint;

    typedef struct LINEPARAM {
        MODELFLOAT W;
		MODELFLOAT C1;
        MODELFLOAT C2;
        MODELFLOAT C3;
    } LineParam;

	typedef union _MODELVEC3D
	{
		struct { MODELFLOAT x, y, z;};
		MODELFLOAT v[3];
	} MODELVEC3D;

    typedef struct SOLUTION {
        MODELVEC3D position;
        MODELVEC3D angle;
        MODELVEC3D angular_velocity;
        MODELVEC3D velocity;
		MODELFLOAT theta;
		MODELFLOAT omega;
		MODELFLOAT t;
        LineParam lineparam;
	} Solution;

	typedef union _MODELLINE
	{
		struct {
			MODELVEC3D a;
			MODELVEC3D b;
		};
		MODELVEC3D v[2];
	} MODELLINE;

	typedef union _MODELRECT
	{
		struct {
			MODELVEC3D bottom_left;
			MODELVEC3D bottom_right;
			MODELVEC3D top_right;
            MODELVEC3D top_left;
		};
		MODELVEC3D v[4];
	} MODELRECT;
	
	typedef union _MODELPLANE
	{
		struct {
			MODELVEC3D point;
			MODELVEC3D normal;
		};
		MODELVEC3D v[2];
	} MODELPLANE;
	
	typedef union _MODELVERTEX
	{
		struct {
			MODELVEC3D point;
			MODELVEC3D normal;
			ModelPoint texcoord;
		};
		MODELVEC3D v[3];
	} MODELVERTEX;
	
	typedef union _MODELTRIANGLE
	{
		struct {
			MODELVERTEX a;
			MODELVERTEX b;
			MODELVERTEX c;
		};
		MODELVERTEX v[3];
	} MODELTRIANGLE;
}

#endif
