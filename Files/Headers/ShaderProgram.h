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

namespace SweepingBirds
{
  /**
   * @enum  ShaderProgramType
   *
   * @brief Values that represent shader program types.
   */
	enum ShaderProgramType
	{
		MAIN,
		BLIT,
		LIGHT,
		BLIT_POINT_LIGHT,
		SHADOW_MAP,
		GAMMA,
		SOBEL,
		GROUND,
		BIRD,
		PREDATOR,
		SKYBOX
	};
	class ShaderProgram
	{
	public:
    /**
     * @fn  ShaderProgram::ShaderProgram();
     *
     * @brief Default constructor.
     */
		ShaderProgram();
		~ShaderProgram();

    /**
     * @fn  void ShaderProgram::add_shader(GLenum shaderType, std::string fileName);
     *
     * @brief Adds a shader to 'fileName'.
     *
     * @param shaderType  Type of the shader.
     * @param fileName    Filename of the file.
     */
		void add_shader(GLenum shaderType, std::string fileName);

    /**
     * @fn  void ShaderProgram::compile();
     *
     * @brief Compiles this object.
     */
		void compile();

    /**
     * @fn  GLuint ShaderProgram::get_program() const
     *
     * @brief Gets the program.
     *
     * @return  The program.
     */
		GLuint get_program() const { return m_gluiProgramObject; }

    /**
     * @fn  void ShaderProgram::add_var(std::string var_name);
     *
     * @brief Adds a variable.
     *
     * @param var_name  Name of the variable.
     */
		void add_var(std::string var_name);

    /**
     * @fn  template<class T> void ShaderProgram::set_var_value(std::string var_name, T value);
     *
     * @brief Sets variable value.
     *
     * @tparam  T Generic type parameter.
     * @param var_name  Name of the variable.
     * @param value     The value.
     */
		template<class T>
		void set_var_value(std::string var_name, T value);

    /**
     * @fn  template<> void ShaderProgram::set_var_value(std::string var_name, int value);
     *
     * @brief Sets variable value.
     *
     * @param var_name  Name of the variable.
     * @param value     The value.
     */
		template<>
		void set_var_value(std::string var_name, int value);

    /**
     * @fn  template<> void ShaderProgram::set_var_value(std::string var_name, float value);
     *
     * @brief Sets variable value.
     *
     * @param var_name  Name of the variable.
     * @param value     The value.
     */
		template<>
		void set_var_value(std::string var_name, float value);

    /**
     * @fn  template<> void ShaderProgram::set_var_value(std::string var_name, float * value);
     *
     * @brief Sets variable value.
     *
     * @param var_name        Name of the variable.
     * @param [in,out]  value If non-null, the value.
     */
		template<>
		void set_var_value(std::string var_name, float * value);

    /**
     * @fn  template<> void ShaderProgram::set_var_value(std::string var_name, const float * value);
     *
     * @brief Sets variable value.
     *
     * @param var_name  Name of the variable.
     * @param value     The value.
     */
		template<>
		void set_var_value(std::string var_name, const float * value);

	private:
    /**
     * @fn  int ShaderProgram::check_link_error(GLuint program);
     *
     * @brief Shader utils.
     *
     * @param program The program.
     *
     * @return  An int.
     */
		int check_link_error(GLuint program);

    /**
     * @fn  int ShaderProgram::check_compile_error(GLuint shader, const char ** sourceBuffer);
     *
     * @brief Check compile error.
     *
     * @param shader        The shader.
     * @param sourceBuffer  Buffer for source data.
     *
     * @return  An int.
     */
		int check_compile_error(GLuint shader, const char ** sourceBuffer);

    /**
     * @fn  GLuint ShaderProgram::compile_shader(GLenum shaderType, const char * sourceBuffer);
     *
     * @brief Compile shader.
     *
     * @param shaderType    Type of the shader.
     * @param sourceBuffer  Buffer for source data.
     *
     * @return  A GLuint.
     */
		GLuint compile_shader(GLenum shaderType, const char * sourceBuffer);

    /**
     * @fn  GLuint ShaderProgram::compile_shader_from_file(GLenum shaderType, const char * fileName);
     *
     * @brief Compile shader from file.
     *
     * @param shaderType  Type of the shader.
     * @param fileName    Filename of the file.
     *
     * @return  A GLuint.
     */
		GLuint compile_shader_from_file(GLenum shaderType, const char * fileName);

    /**
     * @fn  char * ShaderProgram::strsep_custom(char **stringp, const char *delim);
     *
     * @brief Strsep custom.
     *
     * @param [in,out]  stringp If non-null, the stringp.
     * @param delim             The delimiter.
     *
     * @return  null if it fails, else a char*.
     */
		char * strsep_custom(char **stringp, const char *delim);

	protected:
		std::map<std::string, GLuint > m_mProgramVars;
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

}