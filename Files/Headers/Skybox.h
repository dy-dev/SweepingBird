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

#ifndef SKYBOX_H
#define	SKYBOX_H

#include <ogldev_camera.h>
#include <ogldev_cubemap_texture.h>
#include <OGLDevMesh.h>


namespace SweepingBirds
{
	class SkyboxTechnique;
	class Camera;
	class Textured3DObject;
	class TextureManager;

	class SkyBox
	{
	public:
    /**
     * @fn  SkyBox::SkyBox(const OVGCamera* pCamera, TextureManager * texmgr, const PersProjInfo& p);
     *
     * @brief Constructor.
     *
     * @param pCamera           The camera.
     * @param [in,out]  texmgr  If non-null, the texmgr.
     * @param p                 The const PersProjInfo&amp; to process.
     */
		SkyBox(const OVGCamera* pCamera, TextureManager * texmgr, const PersProjInfo& p);

		~SkyBox();

      /**
       * @fn  bool SkyBox::Init(const string& Directory, const string& PosXFilename, const string& NegXFilename, const string& PosYFilename, const string& NegYFilename, const string& PosZFilename, const string& NegZFilename);
       *
       * @brief Initialises this object.
       *
       * @param Directory     Pathname of the directory.
       * @param PosXFilename  Filename of the position x coordinate file.
       * @param NegXFilename  Filename of the negative x coordinate file.
       * @param PosYFilename  Filename of the position y coordinate file.
       * @param NegYFilename  Filename of the negative y coordinate file.
       * @param PosZFilename  Filename of the position z coordinate file.
       * @param NegZFilename  Filename of the negative z coordinate file.
       *
       * @return  true if it succeeds, false if it fails.
       */
		  bool Init(const string& Directory,
			const string& PosXFilename,
			const string& NegXFilename,
			const string& PosYFilename,
			const string& NegYFilename,
			const string& PosZFilename,
			const string& NegZFilename);

    /**
     * @fn  void SkyBox::Render();
     *
     * @brief Renders this object.
     */
		void Render();

	private:
		SkyboxTechnique* m_pSkyboxTechnique;
		const OVGCamera* m_pCamera;
		//const Camera* m_pCamera;
		CubemapTexture* m_pCubemapTex;
		//OGLDevMesh* m_pMesh;
		Textured3DObject * m_pTexturedObject;
		TextureManager * m_pTextureMgr;
		PersProjInfo m_persProjInfo;
	};

}
#endif	/* SKYBOX_H */

