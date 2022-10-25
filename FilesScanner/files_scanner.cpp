#include "files_scanner.hpp"

#include <algorithm>

std::list<std::filesystem::path> FilesScanner::scan_files_in_directory(const std::filesystem::path& directory) {
    std::list<std::filesystem::path> files;

    for(const auto& file : std::filesystem::recursive_directory_iterator(directory)) {
        files.emplace_back(file.path());
    }

    return files;
}

std::list<std::filesystem::path> FilesScanner::filter_files_based_on_extension(const std::list<std::filesystem::path>& files,  const std::filesystem::path& extension) {
    std::list<std::filesystem::path> ret;
    std::copy_if(files.begin(), files.end(), std::back_inserter(ret),
        [&extension](const std::filesystem::path& fn) {
            return fn.extension() != extension;
        }
    );
    return ret;
}
