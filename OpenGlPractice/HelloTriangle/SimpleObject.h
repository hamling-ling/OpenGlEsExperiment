#pragma once

#include <GL/glew.h>

#define MAX_VERTICES	64

class SimpleObject
{
private:
	GLuint g_bufferObject;
	GLuint g_vertexArrayObject;
	GLuint m_verticesLen;

public:
	SimpleObject();
	~SimpleObject(void);
	GLuint GetVertexArrayObject();
	GLuint GetVertexArrayLen();
	void BindBuffer(GLint colorLocation, GLint vertexLocation, const GLfloat *colorsAndVertices, GLuint verticesLen);
};

