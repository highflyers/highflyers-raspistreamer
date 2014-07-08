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
	server = std::make_shared<GstUDPServer>(
			rvw
#if TEST_APP
			, VideoSource::TEST
#endif
	);
	parser_initialize(&parser);
	set_rvw_config();
	set_server_config();
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
#if !(TEST_APP)
	if (!rvw.start())
		return; // TODO
#endif

	server->Setup();
	server->Play();

	while (true)
	{
		// todo read from uart
		byte b;
		std::cin >> b;
		parser_append_byte(&parser, b);
		std::cout << "dodalem bajt";
		if (!parser_has_frame(&parser))
			continue;

		FrameProxy proxy = parser_get_last_frame_ownership(&parser);

		switch (proxy.type)
		{
		case T_StreamerInfo:
			update_stream_info(static_cast<StreamerInfo*>(proxy.pointer));
			server->Setup();
			server->Play();
			break;
		default:
			break;
		}
		frame_proxy_free(&proxy);
	}
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
