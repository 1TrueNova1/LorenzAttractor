#include "IndexBuffer.h"

#include "Debugger.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage)
	: m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	debug(glGenBuffers(1, &m_id));
	debug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	debug(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage));
}

IndexBuffer::~IndexBuffer()
{
	debug(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::bind() const
{
	debug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::unbind() const
{
	debug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}