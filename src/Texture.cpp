#include "Texture.h"

#include "stb_image/stb_image.h"

#include "Debugger.h"

Texture::Texture()
	: m_id(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	std::cout << "Texture default constructor worked\n";
}

Texture::Texture(const std::string& path)
	: m_id(0), m_filePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	load(path);
	std::cout << "Texture constructor with path worked\n";
}

Texture::Texture(const Texture& texture)
	: m_id(texture.m_id), m_filePath(texture.m_filePath), m_localBuffer(texture.m_localBuffer), m_width(texture.m_width), m_height(texture.m_height), m_bpp(texture.m_bpp)
{
	std::cout << "Texture copy constructor worked\n";
}

Texture::Texture(Texture&& texture) noexcept
	: m_id(texture.m_id), m_filePath(texture.m_filePath), m_localBuffer(texture.m_localBuffer), m_width(texture.m_width), m_height(texture.m_height), m_bpp(texture.m_bpp)
{
	std::cout << "Texture move constructor worked\n";
	texture.m_id = 0;
	texture.m_filePath.clear();
	texture.m_localBuffer = nullptr;
	texture.m_width = 0;
	texture.m_height = 0;
	texture.m_bpp = 0;
}

Texture::~Texture()
{
	if(m_id != 0)
		debug(glDeleteTextures(1, &m_id));
}

//Texture& Texture::operator=(const Texture& texture)
//{
//	std::cout << "Texture copy assign worked\n";
//	this->m_id = texture.m_id;
//	this->m_filePath = texture.m_filePath;
//	this->m_localBuffer = texture.m_localBuffer;
//	this->m_width = texture.m_width;
//	this->m_height = texture.m_height;
//	this->m_bpp = texture.m_bpp;
//
//	return *this;
//}

Texture& Texture::operator=(Texture&& texture) noexcept
{
	std::cout << "Texture move assign worked\n";
	this->m_id = texture.m_id;
	this->m_filePath = texture.m_filePath;
	this->m_localBuffer = texture.m_localBuffer;
	this->m_width = texture.m_width;
	this->m_height = texture.m_height;
	this->m_bpp = texture.m_bpp;

	texture.m_id = 0;
	texture.m_filePath.clear();
	texture.m_localBuffer = nullptr;
	texture.m_width = 0;
	texture.m_height = 0;
	texture.m_bpp = 0;
	
	return *this;
}

void Texture::load(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	debug(glGenTextures(1, &m_id));
	debug(glBindTexture(GL_TEXTURE_2D, m_id));

	debug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	debug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	debug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	debug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	debug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
	else
		std::cout << "Something happened while loading image\n";
}

void Texture::bind(unsigned int slot) const
{
	debug(glActiveTexture(GL_TEXTURE0 + slot));
	debug(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::unbind() const
{
	debug(glBindTexture(GL_TEXTURE_2D, 0));
}
