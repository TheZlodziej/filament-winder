/* 
    This library works with CubeIDE FATFS library (http://elm-chan.org/fsw/ff/00index_e.html) 
    Author: Jakub Wicher
*/

#pragma once
#ifndef __FILES_SCANNER__
#define __FILES_SCANNER__

#include <list>
#include <string>

#include "fatfs.h"

class FilesScanner {
    /* variables */
private:
    const static char _directory_separator = '/';

    /* methods */
private:
    static std::string join_paths(const std::string& path1, const std::string& path2);
    static FRESULT read_directory(const std::string& path, std::list<std::string>& save_to);
public:
    static std::list<std::string> scan_files_in_directory(const std::string& path);
    static std::list<std::string> filter_files_based_on_extension(const std::list<std::string>& files,  const std::string& extension); /* removes files from given list that do not have given extension */
    
    /* constructors */
public:
    FilesScanner() = delete;
};

#endif /* __FILES_SCANNER__ */
