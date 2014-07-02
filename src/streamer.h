/*
 * streamer.h
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

#ifndef STREAMER_H_
#define STREAMER_H_

#include "config.h"
extern "C" { // todo it should be moved to a library
#include <parser.h>
}
#include <udpstreamer.h>

class Streamer
{
private:
	RaspiVidWrapper rvw;
	std::shared_ptr<GstUDPServer> server;
	HighFlyersParser parser;
	Config config;

	void set_rvw_config();
	void set_server_config();
	void update_stream_info(const StreamerInfo* info);

public:
	Streamer(int argc, char** argv);
	~Streamer();

	void start();

};



#endif /* STREAMER_H_ */
