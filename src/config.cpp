/*
 * config.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

#include "config.h"
#include <string>
#include <algorithm>

#define INITIAL_CONFIG_LOADER_DEF(config_value, conf_str, default_value) \
		tmp = get_cmd_option(argv, argv + argc, conf_str); \
		config_value = (tmp != nullptr) ? atoi(tmp) : -default_value;

#define INITIAL_CONFIG_LOADER(config_value, conf_str) \
	INITIAL_CONFIG_LOADER_DEF(config_value, conf_str, -1)

Config::Config()
{}

Config::Config(int argc, char** argv)
{
	char* tmp;
	INITIAL_CONFIG_LOADER(bitrate, "-b")
	INITIAL_CONFIG_LOADER(port, "--port")
	INITIAL_CONFIG_LOADER_DEF(mpl_port, "--mpl-port", 5009)
	INITIAL_CONFIG_LOADER_DEF(mpl_port, "--max-mavlink-cnt", 15)
	INITIAL_CONFIG_LOADER_DEF(mpl_port, "--max-udp-cnt", 8192)
	INITIAL_CONFIG_LOADER(width, "-w")
	INITIAL_CONFIG_LOADER(height, "-h")
	INITIAL_CONFIG_LOADER(framerate, "-fps")
	INITIAL_CONFIG_LOADER_DEF(serial_speed, "--serial-speed", 192000)

	vertical_flip = cmd_option_exists(argv, argv + argc, "-vf");
	horizontal_flip = cmd_option_exists(argv, argv + argc, "-hf");

	tmp = get_cmd_option(argv, argv + argc, "--ip");
	ip = (tmp) ? tmp : "";

	tmp = get_cmd_option(argv, argv + argc, "--mpl-ip");
	mpl_ip = (tmp) ? tmp : "192.168.1.105";
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

