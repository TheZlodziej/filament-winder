#include "main_program.hpp"

#include <fstream>

MainProgram::MainProgram():
    _stopped(false),
    _paused(false)
{}

void MainProgram::run() {
    auto files = FilesScanner::filter_files_based_on_extension(
        FilesScanner::scan_files_in_directory(__FLASH_DIR),
        ".gcode");

    // _display.show_files(files);
    std::filesystem::path& chosen = files.front(); //  _display.get_chosen_file();
    std::ifstream stream(chosen);
    if(stream) {
        GCodeParser parser(stream, __GCODE_CONFIG);
        GCodeParser::GCode_Line gc_line{};
        while(parser >> gc_line && !_stopped) {
            while(_paused && !_stopped); // wait for unpause
            execute_gcode(gc_line);
        }
    }

    stream.close();
}

void MainProgram::stop_current_program() {
    _stopped = true;
}

void MainProgram::resume() {
    _paused = false;
}

void MainProgram::pause() {
    _paused = true;
}

void MainProgram::execute_gcode(const GCodeParser::GCode_Line& line) {
    if(line.cmd == "G1") {
        // for(const auto& arg : line.args) {
        //     if(arg.name == 'X') {
        //         move_x_to(arg.value);   
        //     }  
        // }
        // do ...
    }
}
