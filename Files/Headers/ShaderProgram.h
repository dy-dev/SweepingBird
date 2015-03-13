#pragma once
#include "glew/glew.h"
#include <map>

enum openGLVarType
{
	I1,
	MATRIX4FV,
	F1,
	FV3
};

enum ShaderProgramType
{
	MAIN,
	BLIT,
	LIGHT,
	BLIT_POINT_LIGHT,
	SHADOW_MAP,
	GAMMA,
	SOBEL
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void add_shader(GLenum shaderType, std::string fileName);
	void compile();
	GLuint get_program() const { return m_gluiProgramObject; }

	void add_var(std::string var_name);

	template<class T>
	void set_var_value(std::string var_name, T value);
	template<>
	void set_var_value(std::string var_name, int value);
	template<>
	void set_var_value(std::string var_name, float value);
	template<>
	void set_var_value(std::string var_name, float * value);
	template<>
	void set_var_value(std::string var_name, const float * value);

private:
	// Shader utils
	int check_link_error(GLuint program);
	int check_compile_error(GLuint shader, const char ** sourceBuffer);
	GLuint compile_shader(GLenum shaderType, const char * sourceBuffer);
	GLuint compile_shader_from_file(GLenum shaderType, const char * fileName);

	

	char * strsep_custom(char **stringp, const char *delim);

protected:
	std::map<std::string,GLuint > m_mProgramVars;
	std::map < GLenum, std::pair<std::string, GLuint> > m_mShaderFiles;

	GLuint m_gluiProgramObject;
};

template<>
void ShaderProgram::set_var_value(std::string var_name, int value)
{
	auto it = m_mProgramVars.find(var_name);
	if (it != m_mProgramVars.end())
	{
		glProgramUniform1i(m_gluiProgramObject, it->second, value);
	}
}

template<>
void ShaderProgram::set_var_value(std::string var_name, float value)
{
	auto it = m_mProgramVars.find(var_name);
	if (it != m_mProgramVars.end())
	{
		glProgramUniform1f(m_gluiProgramObject, it->second, value);
	}
}

template<>
void ShaderProgram::set_var_value(std::string var_name, float * value)
{
	auto it = m_mProgramVars.find(var_name);
	if (it != m_mProgramVars.end())
	{
		glProgramUniformMatrix4fv(m_gluiProgramObject, it->second, 1, 0, value);
	}
}


template<>
void ShaderProgram::set_var_value(std::string var_name, const float * value)
{
	auto it = m_mProgramVars.find(var_name);
	if (it != m_mProgramVars.end())
	{
		glProgramUniform3fv(m_gluiProgramObject, it->second, 1, value);
	}
}

