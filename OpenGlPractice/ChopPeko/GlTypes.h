//
//  GlTypes.h
//  GameSkeleton
//
//  Created by Nobuhiro Kuroiwa on 12/03/11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef GlTypes_h
#define GlTypes_h

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct texCoord
{
    GLfloat		u;
    GLfloat		v;
};
typedef struct texCoord texCoord;
typedef texCoord* texCoordPtr;

typedef struct vec2 vec2;
typedef vec2* vec2Ptr;

struct vec3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

typedef struct vec3 vec3;
typedef vec3* vec3Ptr;

struct vec4
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
};

typedef struct vec4 vec4;
typedef vec4* vec4Ptr;

struct vertexDataTextured
{
	vec3		vertex;
	vec3		normal;
	texCoord	texCoord;
};
typedef struct vertexDataTextured vertexData;
typedef vertexData* vertexDataPtr;

#endif


