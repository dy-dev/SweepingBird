#include <iostream>
#include <cassert>

#include "ShaderProgram.h"

using namespace SweepingBirds;

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::add_shader(GLenum shaderType, std::string fileName)
{
	m_mShaderFiles[shaderType] = make_pair(fileName, 0);
}

void ShaderProgram::compile()
{
	m_gluiProgramObject = glCreateProgram();
	for each (auto shader in m_mShaderFiles)
	{
		shader.second.second = compile_shader_from_file(shader.first, shader.second.first.c_str());		
		glAttachShader(m_gluiProgramObject, shader.second.second);
	}
	glLinkProgram(m_gluiProgramObject);
  if (check_link_error(m_gluiProgramObject) < 0)
  {
    assert(false && "Shaders compilation failed");
    exit(1);
  }
}

void ShaderProgram::add_var(std::string var_name)
{
	GLuint var_location = glGetUniformLocation(m_gluiProgramObject, var_name.c_str());
	m_mProgramVars[var_name] = var_location;
}

int ShaderProgram::check_compile_error(GLuint shader, const char ** sourceBuffer)
{
	// Get error log size and print it eventually
	int logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		char * log = new char[logLength];
		glGetShaderInfoLog(shader, logLength, &logLength, log);
		char *token, *string;
		string = _strdup(sourceBuffer[0]);
		int lc = 0;
		while ((token = strsep_custom(&string, "\n")) != NULL)
		{
			printf("%3d : %s\n", lc, token);
			++lc;
		}
		fprintf(stderr, "Compile : %s", log);
		delete[] log;
	}
	// If an error happend quit
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
		return -1;
	return 0;
}

int ShaderProgram::check_link_error(GLuint program)
{
	// Get link error log size and print it eventually
	int logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		char * log = new char[logLength];
		glGetProgramInfoLog(program, logLength, &logLength, log);
		fprintf(stderr, "Link : %s \n", log);
 		delete[] log;
	}
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
		return -1;
	return 0;
}

GLuint ShaderProgram::compile_shader(GLenum shaderType, const char * sourceBuffer)
{
	GLuint shaderObject = glCreateShader(shaderType);
	const char * sc[1] = { sourceBuffer };
	glShaderSource(shaderObject, 1, sc,NULL);
	glCompileShader(shaderObject);
	check_compile_error(shaderObject, sc);
	return shaderObject;
}

GLuint ShaderProgram::compile_shader_from_file(GLenum shaderType, const char * path)
{
	FILE *shaderFileDesc; 
	fopen_s(&shaderFileDesc, path, "rb");
	if (!shaderFileDesc)
		return 0;
	fseek(shaderFileDesc, 0, SEEK_END);
	long fileSize = ftell(shaderFileDesc);
	rewind(shaderFileDesc);
	char * buffer = new char[fileSize + 1];
	fread(buffer, 1, fileSize, shaderFileDesc);
	buffer[fileSize] = '\0';
	GLuint shaderObject = compile_shader(shaderType, buffer);
	delete[] buffer;
	return shaderObject;
}


// No windows implementation of strsep
char *  ShaderProgram::strsep_custom(char **stringp, const char *delim)
{
	register char *s;
	register const char *spanp;
	register int c, sc;
	char *tok;
	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;)
	{
		c = *s++;
		spanp = delim;
		do
		{
			if ((sc = *spanp++) == c)
			{
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
}
