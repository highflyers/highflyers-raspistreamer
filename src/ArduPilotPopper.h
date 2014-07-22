/*
 * ArduPilotPopper.h
 *
 *  Created on: Jul 21, 2014
 *      Author: loganek
 */

#ifndef ARDUPILOTPOPPER_H_
#define ARDUPILOTPOPPER_H_

#include <thread>

class ArduPilotPopper
{
private:
	std::thread streamer;
	std::thread writer;
	int fd;

	static const int max_size = 2048;
	unsigned char buffer[max_size];

	int read_from_uart();
	void write_to_uart(unsigned char c);
	void stream_to_ground();
	void write_to_quadro();\
public:
	void open();
	void run();
};


#endif /* ARDUPILOTPOPPER_H_ */
