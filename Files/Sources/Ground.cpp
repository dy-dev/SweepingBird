
#include <Ground.h>
#include <Ground3D.h>
#include <SDL_Image.h>
#include <Mesh.h>
#include <ObjectManager.h>

#include <future>
#include <iostream>

using namespace SweepingBirds;


Ground::Ground()
{
}


Ground::~Ground()
{
}

void Ground::set_ground_3d(Ground3D* newGround)
{
	normalMapSize = newGround->get_meshes()[0]->get_normals().size() * ObjectManager::s_iPatchNumber;
	normalMap = new float[normalMapSize];
	memset(normalMap, 0, normalMapSize);
	heightMapSize = newGround->get_meshes()[0]->get_vertices().size() * ObjectManager::s_iPatchNumber / 3;
	heightMap = new float[heightMapSize];
	memset(heightMap, 0, heightMapSize);
	m_pGround3D = newGround;

	//	m_ppThread= new std::thread*[ObjectManager::s_iPatchNumber];
}

float Ground::get_ground_height()
{
	assert(m_pGround3D);
	return m_pGround3D->get_height();
}

float Ground::Noise(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - float(t) * 0.931322574615478515625e-9;/// 1073741824.0);
}

float Ground::Interpolate(float x, float y, float a)
{
	float negA = 1.0 - a;
	float negASqr = negA * negA;
	float fac1 = 3.0 * (negASqr)-2.0 * (negASqr * negA);
	float aSqr = a * a;
	float fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

	return x * fac1 + y * fac2; //add the weighted factors
}

float Ground::GetValue(float x, float y)
{
	int Xint = int(x);
	int Yint = int(y);
	float Xfrac = x - Xint;
	float Yfrac = y - Yint;

	//noise values
	float n01 = Noise(Xint - 1, Yint - 1);
	float n02 = Noise(Xint + 1, Yint - 1);
	float n03 = Noise(Xint - 1, Yint + 1);
	float n04 = Noise(Xint + 1, Yint + 1);
	float n05 = Noise(Xint - 1, Yint);
	float n06 = Noise(Xint + 1, Yint);
	float n07 = Noise(Xint, Yint - 1);
	float n08 = Noise(Xint, Yint + 1);
	float n09 = Noise(Xint, Yint);

	float n12 = Noise(Xint + 2, Yint - 1);
	float n14 = Noise(Xint + 2, Yint + 1);
	float n16 = Noise(Xint + 2, Yint);

	float n23 = Noise(Xint - 1, Yint + 2);
	float n24 = Noise(Xint + 1, Yint + 2);
	float n28 = Noise(Xint, Yint + 2);

	float n34 = Noise(Xint + 2, Yint + 2);

	//find the noise values of the four corners
	float x0y0 = 0.0625*(n01 + n02 + n03 + n04) + 0.125*(n05 + n06 + n07 + n08) + 0.25*(n09);
	float x1y0 = 0.0625*(n07 + n12 + n08 + n14) + 0.125*(n09 + n16 + n02 + n04) + 0.25*(n06);
	float x0y1 = 0.0625*(n05 + n06 + n23 + n24) + 0.125*(n03 + n04 + n09 + n28) + 0.25*(n08);
	float x1y1 = 0.0625*(n09 + n16 + n28 + n34) + 0.125*(n08 + n14 + n06 + n24) + 0.25*(n04);

	//interpolate between those values according to the x and y fractions
	float v1 = Interpolate(x0y0, x1y0, Xfrac); //interpolate in x direction (y)
	float v2 = Interpolate(x0y1, x1y1, Xfrac); //interpolate in x direction (y+1)
	float fin = Interpolate(v1, v2, Yfrac);  //interpolate in y direction

	return fin;
}

float Ground::Total(float i, float j)
{
	//properties of one octave (changing each loop)
	float t = 0.0f;
	float _amplitude = 2;
	float freq = 0.0005;


	t += GetValue(j * freq, i * freq) * _amplitude;
	_amplitude /= 4;
	freq *= 2;

	t += GetValue(j * freq, i * freq) * _amplitude;
	_amplitude /= 4;
	freq *= 2;

	return t;
}

void setPixel(SDL_Surface *ret, int x, int y, Uint32 coul)
{
	*((Uint32*)(ret->pixels) + x + y * ret->w) = coul;
}

void Ground::generate_heigh_map(int nbPatch)
{
	auto verts = m_pGround3D->get_meshes()[0]->get_vertices();
	futures.clear();
	
	int divider = sqrt(nbPatch);

	for (int i = 0; i < nbPatch; i++)
	{
		if (divider == 0)
			divider = 1;

		int xGridCood = i % divider;
		int zGridCood = i / divider;
		futures.push_back(std::async(generate_H_map, this, zGridCood, xGridCood, i, 1800, verts));
	}

	for (auto &e : futures) {
		e.get();
	}


	//
	//	int octaves = 1;
	//
	//	/*SDL_Init(SDL_INIT_VIDEO);
	//// 	/*SDL_Surface *ret = SDL_CreateRGBSurface(SDL_SWSURFACE, 31, 31, 32, 0, 0, 0, 0);//Create an empty image.
	//// */
	//	auto verts = m_pGround3D->get_meshes()[0]->get_vertices();
	//	
	//	for (int i = 0; i < nbPatch; i++)
	//	{
	//		int divider = sqrt(nbPatch);
	//		if (divider == 0)
	//			divider = 1;
	//
	//		int xGridCood = i % divider;
	//		int zGridCood = i / divider;
	//		//Ground::generate_H_map(this, zGridCood, xGridCood, i, 1800, verts);
	//		m_ppThread[i] = new std::thread(&Ground::generate_H_map, this, zGridCood, xGridCood, i, 1800, verts);
	//	}
	//	
	//	//	int color = (int)((getnoise*128.0) + 128.0);//Convert to 0-256 values.
	//	//	if (color > 255)
	//	//	color = 255;
	//	//	if (color < 0)
	//	//	color = 0;
	//	//	auto col = SDL_MapRGB(ret->format, color, color, color);
	//	//	xad = ((i / 3) % 31);
	//	//	zad = ((i / 3) / 31);
	//	//	setPixel(ret, xad, zad, col);
	//	////SDL_SaveBMP(ret, "heighmap.bmp");
	//
	m_pGround3D->update(heightMap, heightMapSize, normalMap, normalMapSize);
}


void Ground::generate_H_map(Ground* ground, int zOffset, int xOffset, int patchIndex, int patchArea, const std::vector<float>& verts)
{
	for (int i = 0; i < verts.size() - 2; i += 3)
	{
		float x = verts[i];
		float z = verts[i + 2];

		double frequency = 0.0005;
		double amplitude = 2;
		double getnoise = 0;
		for (int a = 0; a < 3; a++)//This loops trough the octaves.
		{
			getnoise += ground->GetValue(abs(z - zOffset*patchArea)*frequency, abs(x - xOffset*patchArea)*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
			frequency *= 2;//This increases the frequency with every loop of the octave.
			amplitude /= 4;//This decreases the amplitude with every loop of the octave
		}//It gives a decimal value, you know, between the pixels.Like 4.2 or 5.1

		ground->heightMap[patchIndex*verts.size() / 3 + i/3] = getnoise;
	}

	ground->generate_normals(ground, patchIndex);
}


void Ground::generate_normals(Ground* ground, int patchIndex)
{
	auto triangleList = m_pGround3D->get_meshes()[0]->get_trianglesList();
	auto vertices = m_pGround3D->get_meshes()[0]->get_vertices();

	glm::vec3* vertexNorm = new glm::vec3[vertices.size() / 3];
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		vertexNorm[i] = glm::vec3(0);
	}

	for (int i = 0; i < triangleList.size(); i += 3)
	{
		glm::vec3 vert[3];
		for (int j = 0; j < 3; j++)
		{
			auto index = triangleList[i + j];
			auto xvrt = vertices[index * 3];
			auto zvrt = vertices[index * 3 + 2];
			vert[j] = glm::vec3(xvrt, 0, zvrt);
			vert[j].y = ground->heightMap[patchIndex*vertices.size() / 3 + index] * 2 * ground->get_ground_height();

		}

		auto vec1 = vert[1] - vert[0];
		auto vec2 = vert[2] - vert[0];

		auto norm = glm::cross(glm::normalize(vec1), glm::normalize(vec2));
		vertexNorm[triangleList[i]] += norm;
		vertexNorm[triangleList[i + 1]] += norm;
		vertexNorm[triangleList[i + 2]] += norm;
	}

	for (int i = 0; i < vertices.size() / 3; i++)
	{
		auto normalisedNormal = glm::normalize(vertexNorm[i]);
		ground->normalMap[vertices.size()*patchIndex + 3 * i] = normalisedNormal.x;
		ground->normalMap[vertices.size()*patchIndex + 3 * i + 1] = normalisedNormal.y;
		ground->normalMap[vertices.size()*patchIndex + 3 * i + 2] = normalisedNormal.z;
	}

	delete[] vertexNorm;
	/*SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface *ret = SDL_CreateRGBSurface(SDL_SWSURFACE, 31, 31, 32, 0, 0, 0, 0);//Create an empty image.

	for (int i = 0; i < normals.size()-2 ; i += 3)
	{
	auto col = SDL_MapRGB(ret->format, 255 * normals[i], 255 * normals[i + 1], 255 * normals[i + 2]);
	auto xad = ((i / 3) % 31);
	auto zad = ((i / 3) / 31);
	setPixel(ret, xad, zad, col);
	}
	SDL_SaveBMP(ret, "normals.bmp");
	*/
}
