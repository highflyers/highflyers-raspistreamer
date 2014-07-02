/*
 * config.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

#include "config.h"
#include <string>
#include <algorithm>

#define INITIAL_CONFIG_LOADER(config_value, conf_str) \
	tmp = get_cmd_option(argv, argv + argc, conf_str); \
	config_value = (tmp != nullptr) ? atoi(tmp) : -1;


Config::Config()
{}

Config::Config(int argc, char** argv)
{
	char* tmp;
	INITIAL_CONFIG_LOADER(bitrate, "-b")
	INITIAL_CONFIG_LOADER(port, "--port")
	INITIAL_CONFIG_LOADER(width, "-w")
	INITIAL_CONFIG_LOADER(height, "-h")
	INITIAL_CONFIG_LOADER(framerate, "-fps")

	vertical_flip = cmd_option_exists(argv, argv + argc, "-vf");
	horizontal_flip = cmd_option_exists(argv, argv + argc, "-hf");

	tmp = get_cmd_option(argv, argv + argc, "--ip");
	ip = (tmp) ? tmp : "";
}

char* Config::get_cmd_option(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool Config::cmd_option_exists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

