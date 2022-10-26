#include <iostream>
#include "files_scanner.hpp"

int main() {
    auto files_in_dir = FilesScanner::scan_files_in_directory("c:\\test_dir\\");
    auto gcode_files = FilesScanner::filter_files_based_on_extension(files_in_dir, "gcode");

    for(const auto& f : files_in_dir) {
        std::cout << f << std::endl;
    }

    std::cout << "filtered:" << std::endl;

    for(const auto& f : gcode_files) {
        std::cout << f << std::endl;
    }

    return 0;
}