#include "gcode_parser.hpp"
#include <stdexcept>

const GCodeParser::GCode_Command eof_cmd(1, std::char_traits<char>::eof());

GCodeParser::GCodeParser(FIL& stream) :
	_stream(&stream)
{}

GCodeParser::GCodeParser(FIL& stream, const GCode_Config& config) :
	_config(config),
	_stream(&stream)
{}

std::vector<std::string> GCodeParser::split_line_into_chunks(std::string line) {
	if (line.back() != _config.chunk_separator) {
		line += _config.chunk_separator;
	}

	std::vector<std::string> splitted;
	std::string temp = "";

	for (char c : line) {
		if (c == _config.comment) {
			if (!temp.empty()) {
				splitted.emplace_back(temp);
			}

			break;
		}

		if (c == _config.chunk_separator) {
			if (!temp.empty()) {
				splitted.emplace_back(temp);
			}
			temp.clear();
			continue;
		}

		temp += c;
	}

	return splitted;
}

GCodeParser::GCode_Line GCodeParser::split_chunks_into_gcode_line(const std::vector<std::string>& chunks)
{
	/* cmd */
	GCode_Command cmd = chunks.front();

	/* args */
	std::vector<GCode_Argument> args;
	std::string chunk;

	for (size_t i = 1; i < chunks.size(); i++) {
		GCode_Argument arg{};
		chunk = chunks[i];
		arg.name = chunk[0];
		try {
			arg.value = std::stof(chunk.substr(1));
		}
		catch (const std::invalid_argument&) {
			arg.value = -1;
		}
		args.emplace_back(arg);
	}

	return GCode_Line{ cmd, args };
}

bool GCodeParser::operator>>(GCode_Line& line)
{
	line = read_line();
	return line.cmd != eof_cmd;
}

GCodeParser::GCode_Line GCodeParser::read_line() {
	if (f_eof(_stream)) {
		return GCode_Line{ .cmd = eof_cmd };
	}

	char buffer[_config.max_line_length];
	uint8_t chars_read;

	do {
		f_gets(buffer, _config.max_line_length, _stream);
		chars_read = strlen(buffer);
	} while (chars_read < 2u);
	
	std::string line(buffer, chars_read);
	std::vector<std::string> chunks(split_line_into_chunks(line));

	if (!chunks.size()) {
		return read_line();
	}

	return split_chunks_into_gcode_line(chunks);
}
