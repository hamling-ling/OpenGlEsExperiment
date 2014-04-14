//
//  GlTypes.h
//  GameSkeleton
//
//  Created by Nobuhiro Kuroiwa on 12/03/11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef GlTypes_h
#define GlTypes_h

#ifdef _WIN64
//define something for Windows (64-bit)
#elif _WIN32
//define something for Windows (32-bit)
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#include <OpenGLES/ES2/gl.h>
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#include <OpenGL/gl.h>
#else
// Unsupported platform
#endif
#elif __linux
// linux
#elif __unix // all unices not caught above
// Unix
#elif __posix
// POSIX
#endif


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

typedef enum GLATTRIBUTE
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_COLOR,
    ATTRIB_TEXCOORD0,
    ATTRIB_TEXCOORD1,
    NUM_ATTRIBUTES
} GlAttribute;

#endif


