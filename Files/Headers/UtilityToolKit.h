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
    /**
     * @fn  static bool UtilityToolKit::check_errors(const char* title);
     *
     * @brief Check errors.
     *
     * @param title The title.
     *
     * @return  true if it succeeds, false if it fails.
     */
		static bool check_errors(const char* title);

    /**
     * @fn  static int UtilityToolKit::printOglError(char *file, int line);
     *
     * @brief Print ogl error.
     *
     * @param [in,out]  file  If non-null, the file.
     * @param line            The line.
     *
     * @return  An int.
     */
		static int printOglError(char *file, int line);

    /**
     * @fn  static void UtilityToolKit::logInfo(std::string logString);
     *
     * @brief Logs an information.
     *
     * @param logString The log string.
     */
		static void logInfo(std::string logString);

    /**
     * @fn  static void UtilityToolKit::createAILogger();
     *
     * @brief Creates an i logger.
     */
		static void createAILogger();

    /**
     * @fn  static std::string UtilityToolKit::getBasePath(const std::string& path);
     *
     * @brief Gets base path.
     *
     * @param path  Full pathname of the file.
     *
     * @return  The base path.
     */
		static std::string getBasePath(const std::string& path);

    /**
     * @fn  static std::string UtilityToolKit::getFileName(const std::string& path);
     *
     * @brief Gets file name.
     *
     * @param path  Full pathname of the file.
     *
     * @return  The file name.
     */
		static std::string getFileName(const std::string& path);

	};

}