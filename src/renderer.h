#pragma once

#include <GL/glew.h>

#include "Debugger.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:

public:
	void draw(const VertexArray& vao, const Shader& shader, size_t count, GLenum mode = GL_TRIANGLES) const
	{
		vao.bind();
		shader.bind();

		debug(glDrawArrays(mode, 0, count));
	}
	void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLenum mode = GL_TRIANGLES) const
	{
		vao.bind();
		ibo.bind();
		shader.bind();

		debug(glDrawElements(mode, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
	}

	void clear() const
	{
		debug(glClear(GL_COLOR_BUFFER_BIT));
	}
};

