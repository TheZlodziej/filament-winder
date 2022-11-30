#pragma once
#ifndef __MAIN_PROGRAM__
#define __MAIN_PROGRAM__

#include "files_scanner.hpp"
#include "gcode_parser.hpp"

#define __FLASH_DIR "/"
#define __GCODE_CONFIG { ' ', '[' } /* chunk_separator, comment */
#define __FILE_EXT ".gcode"

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
    void step(); /* this should be called in main while loop */
    // void show_files();
    void run(); /* runs selected program */
    void pause(); /* pauses selected program */
    void resume(); /* resumes program if one was running */
    void stop_current_program(); /* stops program (unable to resume) */

private:
    void execute_gcode(const GCodeParser::GCode_Line& line); /* executes given line of code (router for gcode commands) */

    void read_usb_drive(); /* reads usb drive once, waits for drive to be unplugged to try again */
    void step_usb_drive(); /* checks for usb state, responsible for mounting, unmounting and calling read */
};

#endif /* __MAIN_PROGRAM__ */