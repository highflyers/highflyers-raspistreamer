/*
 * streamer.h
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

#ifndef STREAMER_H_
#define STREAMER_H_

#include "config.h"
#include "datamanager.h"
#include "ArduPilotPopper.h"
#include <udpstreamer.h>

class Streamer
{
private:
	RaspiVidWrapper rvw;
	std::shared_ptr<GstUDPServer> server;
	Config config;
	DataManager dmgr;
	std::shared_ptr<ArduPilotPopper> popper;

	void set_rvw_config();
	void set_server_config();
	void update_stream_info(const StreamerInfo* info);

public:
	Streamer(int argc, char** argv);
	~Streamer();

	void start();

};



#endif /* STREAMER_H_ */
