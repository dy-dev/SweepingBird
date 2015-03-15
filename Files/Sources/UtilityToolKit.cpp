#include <string>
#include <assimp/DefaultLogger.hpp>

#include "glew/glew.h"

#include <UtilityToolKit.h>

using namespace SweepingBirds;

bool UtilityToolKit::check_errors(const char* title)
{
	int error;
	if ((error = glGetError()) != GL_NO_ERROR)
	{
		std::string errorString;
		switch (error)
		{
		case GL_INVALID_ENUM:
			errorString = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errorString = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errorString = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			errorString = "GL_OUT_OF_MEMORY";
			break;
		default:
			errorString = "UNKNOWN";
			break;
		}
		fprintf(stdout, "OpenGL Error(%s): %s\n", errorString.c_str(), title);
	}
	return error == GL_NO_ERROR;
}

int printOglError(char *file, int line)
{

	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n",
			file, line, gluErrorString(glErr));
		retCode = 1;
	}
	return retCode;
}

void UtilityToolKit::logInfo(std::string logString)
{
	//Will add message to File with "info" Tag
	Assimp::DefaultLogger::get()->info(logString.c_str());
}


void UtilityToolKit::createAILogger()
{
	//Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

	// Create a loggr instance for Console Output
	Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);

	// Create a logger instance for File Output (found in project folder or near .exe)
	Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);

	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
}

// ----------------------------------------------------
// VECTOR STUFF
//

std::string UtilityToolKit::getBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}

std::string UtilityToolKit::getFileName(const std::string& path)
{
	size_t posSlash = path.find_last_of("\\/");
	size_t posDot = path.find_last_of(".");
	if (std::string::npos == posSlash && std::string::npos == posDot)
		return "";
	else if (std::string::npos == posSlash)
		return  path.substr(0, posDot);
	else if (std::string::npos == posDot)
		return  path.substr(posSlash + 1, path.length() - posSlash - 1);
	else
		return  path.substr(posSlash + 1, posDot - posSlash - 1);
}

// res = a cross b;
void crossProduct(float *a, float *b, float *res) {

	res[0] = a[1] * b[2] - b[1] * a[2];
	res[1] = a[2] * b[0] - b[2] * a[0];
	res[2] = a[0] * b[1] - b[0] * a[1];
}


// Normalize a vec3
void normalize(float *a) {

	float mag = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);

	a[0] /= mag;
	a[1] /= mag;
	a[2] /= mag;
}

// ----------------------------------------------------------------------------
/*
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

void get_bounding_box_for_node(const aiNode* nd,
	aiVector3D* min,
	aiVector3D* max)

{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max);
	}
}




void get_bounding_box(aiVector3D* min, aiVector3D* max)
{

min->x = min->y = min->z = 1e10f;
max->x = max->y = max->z = -1e10f;
get_bounding_box_for_node(scene->mRootNode, min, max);
}

*/