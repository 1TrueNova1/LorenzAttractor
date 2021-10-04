#include "VertexBuffer.h"

#include "Debugger.h"

VertexBuffer::VertexBuffer()
	: m_size(0), m_capacity(0)
{
	debug(glGenBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(VertexBuffer&& vbo) noexcept
	: m_id(vbo.m_id), m_size(vbo.m_size), m_capacity(vbo.m_capacity)
{
	vbo.m_id = 0;
	vbo.m_size = 0;
	vbo.m_capacity = 0;
}


VertexBuffer::VertexBuffer(const void* data, size_t size, GLenum usage)
	: m_size(size), m_capacity(size)
{
	debug(glGenBuffers(1, &m_id));
	setData(data, size, usage);
}

VertexBuffer::~VertexBuffer()
{
	if (m_id != 0)
		debug(glDeleteBuffers(1, &m_id));
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vbo) noexcept
{
	this->m_id = vbo.m_id;
	this->m_size = vbo.m_size;
	this->m_capacity = vbo.m_capacity;

	vbo.m_id = 0;
	vbo.m_size = 0;
	vbo.m_capacity = 0;

	return *this;
}

void VertexBuffer::setData(const void* data, size_t size, GLenum usage)
{
	m_size = size;
	m_capacity = size;
	debug(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	debug(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

void VertexBuffer::reserve(size_t capacity, GLenum usage)
{
	m_size = 0;
	m_capacity = capacity;
	debug(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	debug(glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, usage));
}

void VertexBuffer::addData(const void* data, size_t size)
{
	if (m_size + size <= m_capacity)
	{
		m_size += size;
	}
	else
	{
		m_capacity = (m_size + size) * 1.5 + 1;
		size_t new_id;

		debug(glBindBuffer(GL_COPY_READ_BUFFER, m_id));
		debug(glGenBuffers(GL_COPY_WRITE_BUFFER, &new_id));
		debug(glBufferData(GL_COPY_WRITE_BUFFER, m_capacity, nullptr, GL_STATIC_DRAW));
		debug(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_size));

		debug(glDeleteBuffers(1, &m_id));
		m_id = new_id;
	}

	debug(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	debug(glBufferSubData(GL_ARRAY_BUFFER, m_size, size, data));
}

void VertexBuffer::addData(const void* data, size_t size, size_t offset)
{
	if (offset + size <= m_capacity)
	{
		m_size = offset + size;
	}
	else
	{
		m_capacity = (offset + size) * 1.5 + 1;
		size_t new_id;

		debug(glBindBuffer(GL_COPY_READ_BUFFER, m_id));
		debug(glGenBuffers(GL_COPY_WRITE_BUFFER, &new_id));
		debug(glBufferData(GL_COPY_WRITE_BUFFER, m_capacity, nullptr, GL_STATIC_DRAW));
		debug(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, offset));

		debug(glDeleteBuffers(1, &m_id));
		m_id = new_id;
	}
	debug(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	debug(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::reallocate(size_t newSize)
{
	if (newSize <= m_capacity)
		return;

}


void VertexBuffer::bind() const
{
	debug(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::unbind() const
{
	debug(glBindBuffer(GL_ARRAY_BUFFER, 0));
}