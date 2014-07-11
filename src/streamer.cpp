/*
* streamer.cpp
*
*  Created on: Jul 2, 2014
*      Author: loganek
*/

#include "streamer.h"

#define SET_IF_NOT_MINUS(VALUE, METHOD) \
	if (config.VALUE!= -1) \
		rvw.METHOD(config.VALUE);

Streamer::Streamer(int argc, char** argv)
: config(argc, argv)
{
	server = std::make_shared<GstUDPServer>(rvw);

	dmgr.set_stream_info_callback([this](StreamerInfo* info){
		server->Stop();
		rvw.framerate(info->framerate);
		server->Setup();
		server->Play();

	});
}

Streamer::~Streamer()
{
	if (!rvw.close())
		return; // todo log or sth.
}

void Streamer::set_rvw_config()
{
	rvw.verticalFlip(config.vertical_flip);
	rvw.horizontalFlip(config.horizontal_flip);

	SET_IF_NOT_MINUS(width, width)
	SET_IF_NOT_MINUS(height, height)
	SET_IF_NOT_MINUS(framerate, framerate)
	SET_IF_NOT_MINUS(bitrate, bitrate)

	rvw.verticalFlip(config.vertical_flip);
	rvw.horizontalFlip(config.horizontal_flip);
}

void Streamer::set_server_config()
{
	if (!config.ip.empty())
		server->ip(config.ip);
	if (config.port != -1)
		server->port(config.port);
}

void Streamer::start()
{
	server->Setup();
	set_rvw_config();
	set_server_config();
	server->Play();

	dmgr.start(config.ip, config.port + 1);
	while (true);
}

void Streamer::update_stream_info(const StreamerInfo* info)
{
	config.height = info->height;
	config.width = info->height;
	config.port = info->port;
	config.ip =
			std::to_string(info->ip.ip1) + "." +
			std::to_string(info->ip.ip2) + "." +
			std::to_string(info->ip.ip3) + "." +
			std::to_string(info->ip.ip4);
}
