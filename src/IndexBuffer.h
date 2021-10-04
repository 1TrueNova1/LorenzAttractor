#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int m_id;
	unsigned int m_count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int id() const { return m_id; }
	inline unsigned int getCount() const { return m_count; }
};

