/*
 * ArduPilotPopper.h
 *
 *  Created on: Jul 21, 2014
 *      Author: loganek
 */

#ifndef ARDUPILOTPOPPER_H_
#define ARDUPILOTPOPPER_H_

#include "config.h"
#include <thread>

class ArduPilotPopper
{
private:
	std::thread streamer;
	std::thread writer;
	int fd = 0;
	Config config;

	unsigned char buffer[8096];

	int read_from_uart();
	void write_to_uart(unsigned char c);
	void stream_to_ground();
	void write_to_quadro();
public:
	ArduPilotPopper(const Config& config);
	void open();
	void run();
};


#endif /* ARDUPILOTPOPPER_H_ */
