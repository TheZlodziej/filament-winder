#pragma once
#ifndef __GCODE_PARSER__
#define __GCODE_PARSER__

#include <vector>
#include <string>
#include <cstdint>

#include "fatfs.h"

class GCodeParser {
	/* typedefs */
public:	
	typedef std::string GCode_Command;
	/* structs */
public:
	struct GCode_Config {
		char chunk_separator = ' '; /* for G1?X1?Y2?Z3 -> chunk_separator = '?' */
		char comment = ';';
		uint16_t max_line_length = 256;
	};

	struct GCode_Argument {
		char name; // e.g. X
		float value; // e.g. 0.1
	};

	struct GCode_Line {
		GCode_Command cmd; // e.g. G1
		std::vector<GCode_Argument> args; // [ X0.1 Y2.137 Z0.0 ]
	};

	/* variables */
private:
	const GCode_Config _config;
	FIL* _stream;

	/* constructors */
public:
	GCodeParser() = delete;
	explicit GCodeParser(FIL& stream);
	explicit GCodeParser(FIL& stream, const GCode_Config& config);

	/* methods */
public:
	/* read one command and its arguments */
	GCode_Line read_line();

private:
	std::vector<std::string> split_line_into_chunks(std::string line);
	GCode_Line split_chunks_into_gcode_line(const std::vector<std::string>& chunks);

	/* operators */
public:
	bool operator>>(GCode_Line& line);
};

#endif /* __GCODE_PARSER__ */
