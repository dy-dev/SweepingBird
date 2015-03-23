#include <Ground.h>
#include <Ground3D.h>
#include <SDL_Image.h>
#include <Mesh.h>

using namespace SweepingBirds;

Ground::Ground()
{
}


Ground::~Ground()
{
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

void Ground::generate_heigh_map(float xmin, float xmax, float zmin, float zmax)
{
	int octaves = 3;
	std::vector<float> heightMap;

	//SDL_Surface *ret = SDL_CreateRGBSurface(SDL_SWSURFACE, 30, 30, 32, 0, 0, 0, 0);//Create an empty image.

	int xad = 0;
	int zad = 0;
	auto verts = m_pGround3D->get_meshes()[0]->get_vertices();
	//SDL_Init(SDL_INIT_VIDEO);
	for (int i = 0; i < verts.size() - 3; i += 3)
	{
		float x = verts[i];
		float z = verts[i + 2];

		/*for (int x = xmin; x < xmax; x+=60)
		{//Loops to loop trough all the pixels
		for (int z = zmax; z > zmin; z-=60)
		{*/
		double frequency = m_pGround3D->m_ftest2;
		double amplitude = m_pGround3D->m_ftest1;
		double persistence = m_pGround3D->get_frequency();
		double getnoise = 0;
		for (int a = 0; a < octaves; a++)//This loops trough the octaves.
		{
			getnoise += GetValue((double)x*frequency, (double)z*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
			frequency *= m_pGround3D->m_iPatchSize;//This increases the frequency with every loop of the octave.
			amplitude *= persistence;//This decreases the amplitude with every loop of the octave
		}//											It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1
		heightMap.push_back((float)getnoise);
		int color = (int)((getnoise*128.0) + 128.0);//Convert to 0-256 values.
		if (color > 255)
			color = 255;
		if (color < 0)
			color = 0;

		/*	auto col = SDL_MapRGB(ret->format, color, color, color );

		xad = ((i / 3) % 30);
		zad = ((i / 3) / 30);
			setPixel(ret, xad, zad , col);*/
		//	zad++;
		//}
		//zad = 0;
		//xad++;
	}
	//SDL_SaveBMP(ret, "heighmap.bmp");
	generate_normals(heightMap);
	m_pGround3D->update(heightMap);
}


// unsigned char h(x, y) returns the height map value at x, y.
// the map is of size width*height
// Vector3 normal[width*height] will contain the calculated normals.
//
// The height map has x, y axes with (0, 0) being the top left corner of the map.
// The resulting mesh is assumed to be in a left hand system with x right, z into the screen
// and y up (i.e. as in DirectX).
//
// yScale denotes the scale of mapping heights to final y values in model space
// (i.e. a height differenbce of 1 in the height map results in a height difference
// of yScale in the vertex coordinate).
// xzScale denotes the same for the x, z axes. If you have different scale factors
// for x, z then the formula becomes
// normal[y*width+x].set(-sx*yScale, 2*xScale, xScalesy*xScale*yScale/zScale);
void Ground::generate_normals(std::vector<float>& heightmap)
{
	std::vector<float> normals;
	//SDL_Init(SDL_INIT_VIDEO);
	for (int i = 0; i < heightmap.size(); i++)
	{
		float sx = heightmap[i % 30 < 29 ? i + 1 : i] - heightmap[i % 30 > 0 ? i - 1 : i];
		float sy = heightmap[i / 30 < 29 ? i + 30 : i] - heightmap[i / 30 > 0 ? i - 30 : i];

		//}
		/*for (unsigned int y = 0; y < height; ++y)
		{
		for (unsigned int x = 0; x < width; ++x)
		{*/
		// The ? : and ifs are necessary for the border cases.

		/*float sx = h(x < width - 1 ? x + 1 : x, y) - h(x0 ? x - 1 : x, y);
		if (x == 0 || x == width - 1)
		sx *= 2;

		float sy = h(x, y < height - 1 ? y + 1 : y) - h(x, y0 ? y - 1 : y);
		if (y == 0 || y == height - 1)
		sy *= 2;
		*/

		glm::vec3 norm(-sx, 2, sy);
		auto normalisedNormal = glm::normalize(norm);
		normals.push_back(normalisedNormal.x);
		normals.push_back(normalisedNormal.y);
		normals.push_back(normalisedNormal.z);
		//}
	}
	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(0);
	m_pGround3D->get_meshes()[0]->set_normals(normals);
	/*SDL_Surface *ret = SDL_CreateRGBSurface(SDL_SWSURFACE, 31, 31, 32, 0, 0, 0, 0);//Create an empty image.
	
	for (int i = 0; i < normals.size() - 3; i += 3)
	{
		auto col = SDL_MapRGB(ret->format, 255 * normals[i], 255 * normals[i + 1], 255 * normals[i + 2]);
		int j = i / 3;
		setPixel(ret, j % 31, j / 31, col);
	}
	SDL_SaveBMP(ret, "normals.bmp");
	*/
}
