/*
 * config.h
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <algorithm>

struct Config
{
	std::string ip;
	int port = -1;
	int width = -1;
	int height = -1;
	int framerate = -1;
	int bitrate = -1;
	bool horizontal_flip = false;
	bool vertical_flip = false;
	int mpl_port = -1;
	std::string mpl_ip;
	int serial_speed = -1;

	Config();
	Config(int argc, char** argv);
	char* get_cmd_option(char** begin, char** end, const std::string& option);
	bool cmd_option_exists(char** begin, char** end, const std::string& option);

};

#endif /* CONFIG_H_ */
