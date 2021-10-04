#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_id;

public:
	VertexArray();
	VertexArray(VertexArray&& vao) noexcept;
	~VertexArray();

	VertexArray& operator=(VertexArray&& vao) noexcept;

	void addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	inline void bind() const;
	inline void unbind() const;

	inline unsigned int id() const { return m_id; }
	inline void setId(unsigned int id) { m_id = id; }
};
