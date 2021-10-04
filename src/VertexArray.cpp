#include "VertexArray.h"

#include "Debugger.h"

VertexArray::VertexArray()
{
	debug(glGenVertexArrays(1, &m_id));
	bind();
}

VertexArray::VertexArray(VertexArray&& vao) noexcept
	: m_id(vao.m_id)
{
	vao.m_id = 0;
}

VertexArray::~VertexArray()
{
	if (m_id != 0)
		debug(glDeleteVertexArrays(1, &m_id));
}

VertexArray& VertexArray::operator=(VertexArray&& vao) noexcept
{
	this->m_id = vao.m_id;
	vao.m_id = 0;

	return *this;
}

void VertexArray::addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	bind();
	vbo.bind();
	const auto& elements = layout.getElements();
	size_t offset = 0;
	for (size_t i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		debug(glEnableVertexAttribArray(i));
		debug(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::getTypeSize(element.type);
	}
}

void VertexArray::bind() const
{
	debug(glBindVertexArray(m_id));
}

void VertexArray::unbind() const
{
	debug(glBindVertexArray(0));
}
