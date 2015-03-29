/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TECHNIQUE_H
#define	TECHNIQUE_H

#include <list>
#include <GL/glew.h>

namespace SweepingBirds
{

	class Technique
	{
	public:
    /**
     * @fn  Technique::Technique();
     *
     * @brief Default constructor.
     */
		Technique();

		virtual ~Technique();

    /**
     * @fn  virtual bool Technique::Init();
     *
     * @brief Initialises this object.
     *
     * @return  true if it succeeds, false if it fails.
     */
		virtual bool Init();

    /**
     * @fn  void Technique::Enable();
     *
     * @brief Enables this object.
     */
		void Enable();

	protected:
    /**
     * @fn  bool Technique::AddShader(GLenum ShaderType, const char* pFilename);
     *
     * @brief Adds a shader to 'pFilename'.
     *
     * @param ShaderType  Type of the shader.
     * @param pFilename   Filename of the file.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool AddShader(GLenum ShaderType, const char* pFilename);

    /**
     * @fn  bool Technique::Finalize();
     *
     * @brief Finalizes this object.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool Finalize();

    /**
     * @fn  GLint Technique::GetUniformLocation(const char* pUniformName);
     *
     * @brief Gets uniform location.
     *
     * @param pUniformName  Name of the uniform.
     *
     * @return  The uniform location.
     */
		GLint GetUniformLocation(const char* pUniformName);

    /**
     * @fn  GLint Technique::GetProgramParam(GLint param);
     *
     * @brief Gets program parameter.
     *
     * @param param The parameter.
     *
     * @return  The program parameter.
     */
		GLint GetProgramParam(GLint param);

		/** @brief The shader prog. */
		GLuint m_shaderProg;

	private:

		typedef std::list<GLuint> ShaderObjList;
		ShaderObjList m_shaderObjList;
	};
}
#endif	/* TECHNIQUE_H */

