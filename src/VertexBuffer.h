#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
	unsigned int m_id;
	size_t m_size;
	size_t m_capacity;

public:
	VertexBuffer();
	VertexBuffer(VertexBuffer&& vbo) noexcept;
	VertexBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
	~VertexBuffer();

	VertexBuffer& operator=(VertexBuffer&& vbo) noexcept;

	void setData(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
	void reserve(size_t size, GLenum usage = GL_STATIC_DRAW);
	void addData(const void* data, size_t size);
	void addData(const void* data, size_t size, size_t offset);
private:
	void reallocate(size_t newSize);

public:

	void bind() const;
	void unbind() const;

	inline unsigned int id() const { return m_id; }
	inline size_t size() const { return m_size; }
	inline size_t capacity() const { return m_capacity; }

	inline void setId(unsigned int id) { m_id = id; }
};

