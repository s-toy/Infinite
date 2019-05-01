#include "meshRenderer.h"

CMeshRenderer::CMeshRenderer(const GLfloat *vVertices, int vVerticesSize)
{
	__setUpMesh(vVertices, vVerticesSize);
	m_VerticesCount = vVerticesSize / (5 * sizeof(GLfloat));
}

//**********************************************************************************************
//FUNCTION:
void CMeshRenderer::draw() 
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_VerticesCount);
	glBindVertexArray(0);
}

//**********************************************************************************************
//FUNCTION:
void CMeshRenderer::__setUpMesh(const GLfloat *vVertices, int vVerticesSize) 
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vVerticesSize, vVertices, GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}