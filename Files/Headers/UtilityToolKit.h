#pragma once
#include <string>
#ifndef DEBUG_PRINT
#define DEBUG_PRINT 1
#endif

#if DEBUG_PRINT == 0
#define debug_print(FORMAT, ...) ((void)0)
#else
#ifdef _MSC_VER
#define debug_print(FORMAT, ...) \
		fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
				__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#else
#define debug_print(FORMAT, ...) \
		fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
				__func__, __FILE__, __LINE__, __VA_ARGS__)
#endif
#endif

namespace SweepingBirds
{

	class UtilityToolKit
	{
	public:
		static bool check_errors(const char* title);
		static int printOglError(char *file, int line);
		static void logInfo(std::string logString);
		static void createAILogger();
		static std::string getBasePath(const std::string& path);
		static std::string getFileName(const std::string& path);

	};

}