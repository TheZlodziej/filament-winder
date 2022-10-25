#pragma once
#ifndef __FILES_SCANNER__
#define __FILES_SCANNER__

#include <list>
#include <filesystem>

class FilesScanner {
    /* methods */
public:
    static std::list<std::filesystem::path> scan_files_in_directory(const std::filesystem::path& directory);
    static std::list<std::filesystem::path> filter_files_based_on_extension(const std::list<std::filesystem::path>& files,  const std::filesystem::path& extension); /* removes files from given list that do not have given extension */

    /* constructors */
public:
    FilesScanner() = delete;
};

#endif /* __FILES_SCANNER__ */
