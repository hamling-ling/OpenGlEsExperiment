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

vector<CTriangle3v>& SimpleObject::GetVertexArray()
{
	return m_vertices;
}

void SimpleObject::BindBuffer(GLint normalLocation, GLint vertexLocation, const GLfloat *normalsAndVertices, GLuint verticesLen)
{
	glGenBuffers(1, &g_bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8 * verticesLen, normalsAndVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (GLvoid *)(0 * sizeof (GLfloat)));
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	
	glEnableVertexAttribArray(normalLocation);
	glEnableVertexAttribArray(vertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_verticesLen = verticesLen;

	for(GLuint i = 0; i < verticesLen; i=i+3) {
		CVertex a(normalsAndVertices);
		normalsAndVertices += 8;
		CVertex b(normalsAndVertices);
		normalsAndVertices += 8;
		CVertex c(normalsAndVertices);
		CTriangle3v tri(a,b,c);

		m_vertices.push_back(tri);
	}
}
