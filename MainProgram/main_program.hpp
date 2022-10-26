#pragma once
#ifndef __MAIN_PROGRAM__
#define __MAIN_PROGRAM__

#include "files_scanner.hpp"
#include "gcode_parser.hpp"

#define __FLASH_DIR "C:/TEST_DIR"
#define __GCODE_CONFIG { ' ', '\n', '[' } /* chunk_separator, new_command_separator, comment */

class MainProgram {
    /* variables */
private:
    bool _stopped;
    bool _paused;    

    /* constructors */
public:
    //MainProgram() = delete;
    MainProgram();
    
    /* methods */
public:
    void run(); // starts the program
    void pause(); // stops the program (after last line execution) and waits to be resumed
    void resume(); // resumes the program if it was interrupted
    void stop_current_program(); // stops execution of current gcode & closes stream

private:
    void execute_gcode(const GCodeParser::GCode_Line& line);
};

#endif /* __MAIN_PROGRAM__ */