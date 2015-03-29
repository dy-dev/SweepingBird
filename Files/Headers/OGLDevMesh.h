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

#ifndef MESH_H
#define	MESH_H

#include <map>
#include <vector>
#include <GL/glew.h>
#include <Importer.hpp>      // C++ importer interface
#include <scene.h>       // Output data structure
#include <postprocess.h> // Post processing flags

#include "ogldev_util.h"
#include "ogldev_math_3d.h"
#include "ogldev_texture.h"

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    /**
     * @fn  Vertex()
     *
     * @brief Default constructor.
     */
    Vertex() {}

    /**
     * @fn  Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
     *
     * @brief Constructor.
     *
     * @param pos     The position.
     * @param tex     The tex.
     * @param normal  The normal.
     */
    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};


class OGLDevMesh
{
public:
  /**
   * @fn  OGLDevMesh::OGLDevMesh();
   *
   * @brief Default constructor.
   */
	OGLDevMesh();

	~OGLDevMesh();

    /**
     * @fn  bool OGLDevMesh::LoadMesh(const std::string& Filename);
     *
     * @brief Loads a mesh.
     *
     * @param Filename  Filename of the file.
     *
     * @return  true if it succeeds, false if it fails.
     */
    bool LoadMesh(const std::string& Filename);

    /**
     * @fn  void OGLDevMesh::Bind();
     *
     * @brief Binds this object.
     */
	  void Bind();

    /**
     * @fn  void OGLDevMesh::Render();
     *
     * @brief Renders this object.
     */
    void Render();

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {

        /**
         * @fn  MeshEntry();
         *
         * @brief Default constructor.
         */
        MeshEntry();

        ~MeshEntry();

        /**
         * @fn  bool Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);
         *
         * @brief Initialises this object.
         *
         * @param Vertices  The vertices.
         * @param Indices   The indices.
         *
         * @return  true if it succeeds, false if it fails.
         */
        bool Init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
};


#endif	/* MESH_H */

