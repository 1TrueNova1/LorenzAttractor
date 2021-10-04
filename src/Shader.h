#pragma once

#include <GL/glew.h>

#include <iostream>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
private:
	unsigned int id;

	std::unordered_map<std::string, int> m_locationCache;

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	static std::string loadShader(const std::string& path);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compileShader(const std::string& source, unsigned int type);

	void bind() const;
	void unbind() const;

	inline unsigned int getId() const { return id; }

	void setUniform1i(const std::string& name, int v0);
	void setUniform2i(const std::string& name, int v0, int v1);
	void setUniform3i(const std::string& name, int v0, int v1, int v2);
	void setUniform4i(const std::string& name, int v0, int v1, int v2, int v3);

	void setUniform1f(const std::string& name, float v0);
	void setUniform2f(const std::string& name, float v0, float v1);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void setUniform1f(const std::string& name, const glm::vec1& vector);
	void setUniform2f(const std::string& name, const glm::vec2& vector);
	void setUniform3f(const std::string& name, const glm::vec3& vector);
	void setUniform4f(const std::string& name, const glm::vec4& vector);

	void setUniformMat2f(const std::string& name, const glm::mat2& matrix);
	void setUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:

	unsigned int getUniformLocation(const std::string& name);
};

