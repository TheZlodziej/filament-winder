#include <iostream>
#include "files_scanner.hpp"

int main() {
    std::list<std::filesystem::path> files = FilesScanner::scan_files_in_directory("c:/test_dir/");
    FilesScanner::filter_files_based_on_extension(files, ".gcode");

    for(const auto& s : files) {
        std::cout << s << std::endl;
    }

    return 0;
}