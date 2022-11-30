#include "main_program.hpp"

#include "usb_host.h"
#include "fatfs.h"

/* FATFS definitions (can be found in fatfs.h)*/
extern char USBHPath[4]; /* USBH logical drive path */
extern FATFS USBHFatFS; /* file system object for USBH logical drive */

/* application state for determining usb drive status */
extern ApplicationTypeDef Appli_state

/* defines for inital use */
#define usb_drive_logic_path USBHPath
#define usb_drive_filesystem USBHFatFS
#define usb_drive_state Appli_state
#define USB_DRIVE_PLUGGED APPLICATION_START
#define USB_DRIVE_READY APPLICATION_READY
#define USB_DRIVE_UNPLUGGED APPLICATION_DISCONNECT

MainProgram::MainProgram():
    _stopped(false),
    _paused(false)
{}

void MainProgram::display_files(const std::list<std::vector>& files) {
    /* ... TODO ... */
}

void MainProgram::read_usb_drive() {
    std::list<std::string> files = FilesScanner::filter_files_based_on_extension(
        FilesScanner::scan_files_in_directory(__FLASH_DIR),
        __FILE_EXT
    );

    // _display.swap_files(files); // swap files in display class
    std::string& chosen = files.front(); // _display.get_chosen_file();
}

void MainProgram::step_usb_drive() {
    static bool already_read = false;
    switch(usb_drive_state) {
        case USB_DRIVE_PLUGGED:
            f_mount(&usb_drive_filesystem, usb_drive_logic_path, 0);
            break;

        case USB_DRIVE_UNPLUGGED:
            f_mount(nullptr, usb_drive_logic_path, 0);
            already_read = false;
            break;

        case USB_DRIVE_READY:
            if(!already_read) {
                read_usb_drive();
                already_read = true;
            }
            break;
    }
}

void MainProgram::step() {
    read_usb_drive();
    // _display.show();
}

void MainProgram::run() {
    std::string& chosen = ""; // _display.get_chosen_file();
    FIL stream;
    FRESULT result = f_open(&stream, chosen.c_str(), FA_READ);
    if(result == FR_OK) {
        GCodeParser parser(stream, __GCODE_CONFIG);
        GCodeParser::GCode_Line gc_line{};
        while(parser >> gc_line && !_stopped) {
            while(_paused && !_stopped); /* wait for unpause */
            execute_gcode(gc_line);
        }
    }

    f_close(&stream);
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
