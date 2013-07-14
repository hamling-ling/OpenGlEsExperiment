#pragma once

#include <vector>
#include <GL/glew.h>
#include "Triangle3v.h"

#define MAX_VERTICES	64

using namespace std;

class SimpleObject
{
private:
	GLuint g_bufferObject;
	GLuint g_vertexArrayObject;
	GLuint m_verticesLen;
	vector<CTriangle3v> m_vertices;

public:
	SimpleObject();
	~SimpleObject(void);
	GLuint GetVertexArrayObject();
	GLuint GetVertexArrayLen();
	vector<CTriangle3v> &GetVertexArray();

	//void BindBuffer(GLint colorLocation, GLint vertexLocation, const GLfloat *normalsAndVertices, GLuint verticesLen);
	void SimpleObject::BindBuffer(GLint normalLocation, GLint vertexLocation, const GLfloat *normalsAndVertices, GLuint verticesLen);
};

