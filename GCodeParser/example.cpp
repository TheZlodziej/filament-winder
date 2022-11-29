//#include <fstream>
//#include <iostream>
//
//#include "gcode_parser.hpp"
//
//int main() {
//	std::ifstream file("Temptower_neu..gcode");
//	if (file.is_open()) {
//		std::cout << "file opened";
//		GCodeParser parser(file);
//		GCodeParser::GCode_Line gc_line;
//		while (parser >> gc_line) {
//			std::cout << "(" << gc_line.cmd << "):\n";
//			for (auto gc_arg : gc_line.args)
//			{
//				std::cout << "\t(" << gc_arg.name << ":" << gc_arg.value << ")\n";
//			}
//		}
//	}
//	file.close();
//}


#include "gcode_parser.hpp"
#include "fatfs.h"

extern UART_HandleTypeDef huart3;

void send_uart(const std::string& msg) {
	HAL_UART_Transmit(&huart3, (uint8_t*)msg.c_str(), msg.size(), 100);
}

int main() {
    FIL file;
    f_open(&file, filename.c_str(), FA_READ);
}

int main() {
    FIL file;
    f_open(&file, filename.c_str(), FA_READ);

    GCodeParser::GCode_Config config{ .comment = '#' };
    GCodeParser parser(file, config);

    GCodeParser::GCode_Line gc_line;
    while (parser >> gc_line) {
        send_uart(gc_line.cmd + ":\t");
        for (auto gc_arg : gc_line.args)
        {
            std::string arg_msg = "(";
            arg_msg += gc_arg.name;
            arg_msg += ":"
            arg_msg += std::to_string(gc_arg.value);
            arg_msg += ")\r\n";
        }
    }

    f_close(&file);
}
