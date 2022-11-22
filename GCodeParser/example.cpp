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

#include <iostream>
#include <sstream>

#include "gcode_parser.hpp"

int main() {
    std::stringstream ss("G11 X1 Y2 Z3\n"
        "G2 X1 Y2; 2138\n"
        "M11 X Y\n"
        "G1 X32 ;Y21"
    );
    GCodeParser::GCode_Config config{ .comment = 'e' };
    GCodeParser parser(ss, config);

    GCodeParser::GCode_Line gc_line;
    while (parser >> gc_line) {
        std::cout << "(" << gc_line.cmd << "):\n";
        for (auto gc_arg : gc_line.args)
        {
            std::cout << "\t(" << gc_arg.name << ":" << gc_arg.value << ")\n";
        }
    }
}
