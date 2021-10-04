#pragma once

#include <GL/glew.h>
#include <iostream>

class Texture
{
private:
	unsigned int m_id;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_bpp;

public:
	Texture();
	Texture(const std::string& path);
	Texture(const Texture& texture);
	Texture(Texture&& texture) noexcept;
	~Texture();

	//Texture& operator=(const Texture& texture);
	Texture& operator=(Texture&& texture) noexcept;

	void load(const std::string& path);

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
};

