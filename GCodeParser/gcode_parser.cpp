#include "gcode_parser.h"

GCodeParser::GCodeParser(std::istream& stream) :
	_stream(stream)
{}

GCodeParser::GCodeParser(std::istream& stream, const GCode_Config& config) :
	_config(config),
	_stream(stream)
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
	GCode_Command cmd{};
	std::vector<GCode_Argument> args;
	std::string chunk = chunks.front();

	/* cmd */
	cmd.name = chunk[0];
	cmd.value = std::stoi(chunk.substr(1));

	/* args */
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
	return line.cmd.name != std::char_traits<char>::eof();
}

GCodeParser::GCode_Line GCodeParser::read_line() {
	if (_stream.peek() == std::char_traits<char>::eof()) {
		return GCode_Line{ GCode_Command { std::char_traits<char>::eof() }, {} };
	}

	std::string line;
	do {
		std::getline(_stream, line, _config.new_command_separator);
	} while (line.size() < 2);
	
	std::vector<std::string> chunks(split_line_into_chunks(line));

	if (!chunks.size()) {
		return read_line();
	}

	return split_chunks_into_gcode_line(chunks);
}