#include "Debugger.h"

#include <iostream>

void GLCLearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL error; code: " << error << " file: " << file << " line: " << line << '\n';
		return false;
	}
	return true;
}