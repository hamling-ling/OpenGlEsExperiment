#include "SimpleObject.h"


SimpleObject::SimpleObject()
{
}


SimpleObject::~SimpleObject(void)
{
	glDeleteVertexArrays(1, &g_vertexArrayObject);
	glDeleteBuffers(1, &g_bufferObject);
}

GLuint SimpleObject::GetVertexArrayObject()
{
	return g_vertexArrayObject;
}

GLuint SimpleObject::GetVertexArrayLen()
{
	return m_verticesLen;
}

void SimpleObject::BindBuffer(GLint colorLocation, GLint vertexLocation, const GLfloat *colorsAndVertices, GLuint verticesLen)
{
	glGenBuffers(1, &g_bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 6 * verticesLen, colorsAndVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), 0);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(vertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_verticesLen = verticesLen;
}
