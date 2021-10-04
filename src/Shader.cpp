#include "Shader.h"

#include "Debugger.h"
#include <fstream>
#include <string>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::string vertexShader = loadShader(vertexShaderPath);
	std::string fragmentShader = loadShader(fragmentShaderPath);

	id = createShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	debug(glDeleteProgram(id));
}

std::string Shader::loadShader(const std::string& path)
{
	std::string shader;
	std::ifstream stream;
	stream.open(path);

	if (!stream) {
		std::cout << "Error occured while loading shader from " << path << '\n';
		return shader;
	}
	std::string tmp;
	while (!stream.eof()) {
		std::getline(stream, tmp);
		shader += tmp;
		shader += '\n';
	}
	std::cout << shader << '\n';
	return shader;
}
unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

	debug(glAttachShader(program, vs));
	debug(glAttachShader(program, fs));
	debug(glLinkProgram(program));
	debug(glValidateProgram(program));

	debug(glDeleteShader(vs));
	debug(glDeleteShader(fs));

	return program;
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* data = source.data();
	debug(glShaderSource(id, 1, &data, nullptr));
	debug(glCompileShader(id));

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		debug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = new char[length];
		debug(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader\n";
		std::cout << message << '\n';

		delete[] message;
	}
	return id;
}

void Shader::setUniform1i(const std::string& name, int v0)
{
	debug(glUniform1i(getUniformLocation(name), v0));
}
void Shader::setUniform2i(const std::string& name, int v0, int v1)
{
	debug(glUniform2i(getUniformLocation(name), v0, v1));
}
void Shader::setUniform3i(const std::string& name, int v0, int v1, int v2)
{
	debug(glUniform3i(getUniformLocation(name), v0, v1, v2));
}
void Shader::setUniform4i(const std::string& name, int v0, int v1, int v2, int v3)
{
	debug(glUniform4i(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniform1f(const std::string& name, float v0)
{
	debug(glUniform1f(getUniformLocation(name), v0));
}
void Shader::setUniform2f(const std::string& name, float v0, float v1)
{
	debug(glUniform2f(getUniformLocation(name), v0, v1));
}
void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
	debug(glUniform3f(getUniformLocation(name), v0, v1, v2));
}
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	debug(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniform1f(const std::string& name, const glm::vec1& vector)
{
	debug(glUniform1f(getUniformLocation(name), vector.x));
}
void Shader::setUniform2f(const std::string& name, const glm::vec2& vector)
{
	debug(glUniform2f(getUniformLocation(name), vector.x, vector.y));
}
void Shader::setUniform3f(const std::string& name, const glm::vec3& vector)
{
	debug(glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z));
}
void Shader::setUniform4f(const std::string& name, const glm::vec4& vector)
{
	debug(glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w));
}

void Shader::setUniformMat2f(const std::string& name, const glm::mat2& matrix)
{
	debug(glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	debug(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	debug(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
	if (m_locationCache.find(name) != m_locationCache.end())
		return m_locationCache[name];

	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << " does not exist\n";
	m_locationCache[name] = location;

	return location;
}


void Shader::bind() const
{
	debug(glUseProgram(id));
}

void Shader::unbind() const
{
	debug(glUseProgram(0));
}

