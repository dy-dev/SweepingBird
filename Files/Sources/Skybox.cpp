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

#include <Skybox.h>
#include <SkyboxTechnique.h>
#include <Textured3DObject.h>
#include <TextureManager.h>
#include <Camera.h>
#include <ogldev_pipeline.h>
#include <Mesh.h>

using namespace SweepingBirds;

SkyBox::SkyBox(const OVGCamera* pCamera, TextureManager * texmgr, const PersProjInfo& p)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;

	m_pSkyboxTechnique = NULL;
	m_pCubemapTex = NULL;
	//m_pMesh = NULL;
	m_pTexturedObject = nullptr;
	m_pTextureMgr = texmgr;
}


SkyBox::~SkyBox()
{
	SAFE_DELETE(m_pSkyboxTechnique);
	SAFE_DELETE(m_pCubemapTex);
	//SAFE_DELETE(m_pMesh);
}


bool SkyBox::Init(const std::string& Directory,
	const std::string& PosXFilename,
	const std::string& NegXFilename,
	const std::string& PosYFilename,
	const std::string& NegYFilename,
	const std::string& PosZFilename,
	const std::string& NegZFilename)
{
	m_pSkyboxTechnique = new SkyboxTechnique();

	if (!m_pSkyboxTechnique->Init()) {
		printf("Error initializing the skybox technique\n");
		return false;
	}

	m_pSkyboxTechnique->Enable();
	m_pSkyboxTechnique->SetTextureUnit(0);

	m_pCubemapTex = new CubemapTexture(Directory,
		PosXFilename,
		NegXFilename,
		PosYFilename,
		NegYFilename,
		PosZFilename,
		NegZFilename);

	if (!m_pCubemapTex->Load()) {
		return false;
	}

	//m_pMesh = new OGLDevMesh();
	m_pTexturedObject = new Textured3DObject();
	m_pTexturedObject->load_object("./Objects/SkyBox/sphere.obj", false, m_pTextureMgr);
	m_pTexturedObject->bind_meshes();

	return true;
	//return m_pMesh->LoadMesh("./Objects/SkyBox/sphere.obj");

}


void SkyBox::Render()
{
	m_pSkyboxTechnique->Enable();

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	Pipeline p;
	p.Scale(100.0f, 100.0f, 100.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(m_pCamera->GetPos().x, m_pCamera->GetPos().y, m_pCamera->GetPos().z);
	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	/*p.WorldPos(m_pCamera->GetEye().x, m_pCamera->GetEye().y, m_pCamera->GetEye().z);
	Vector3f pos = Vector3f(m_pCamera->GetEye().x, m_pCamera->GetEye().y, m_pCamera->GetEye().z);
	Vector3f target = Vector3f(m_pCamera->GetO().x, m_pCamera->GetO().y, m_pCamera->GetO().z);
	Vector3f up = Vector3f(m_pCamera->GetUp().x, m_pCamera->GetUp().y, m_pCamera->GetUp().z);
	Vector3f Pos(0.0f, 1.0f, -20.0f);
	Vector3f Target(0.0f, 0.0f, 1.0f);
	Vector3f Up(0.0, 1.0f, 0.0f);
	p.SetCamera(pos, target, up);*/
	p.SetPerspectiveProj(m_persProjInfo);
	m_pSkyboxTechnique->SetWVP(p.GetWVPTrans());
	m_pCubemapTex->Bind(GL_TEXTURE0);
	
	auto mesh = m_pTexturedObject->get_meshes()[0];
	glBindVertexArray(mesh->get_vao());
	m_pTextureMgr->apply_material(mesh->get_material());

	glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}