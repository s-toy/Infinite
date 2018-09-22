#pragma once

#include <vector>
#include <GL/glew.h>

class CMeshRenderer
{
public:
	CMeshRenderer(const GLfloat *vVertices, int vVerticesSize);

	void draw();

private:
	void __setUpMesh(const GLfloat *vVertices, int vVerticesSize);

private:
	GLuint m_VAO, m_VBO;
	int m_VerticesCount;
};