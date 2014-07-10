/*
* streamer.cpp
*
*  Created on: Jul 2, 2014
*      Author: loganek
*/

#include "streamer.h"
#include <cstring>

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
#include <gstreamermm/appsink.h>
#include <gstreamermm.h>
void Streamer::start()
{
	server->Setup();
	server->Play();

	Glib::RefPtr<Gst::Pipeline> pipe_proto = Gst::Pipeline::create();
	Glib::RefPtr<Gst::Element> udpsrc = Gst::ElementFactory::create_element("udpsrc");
	udpsrc->property<Glib::ustring>("multicast-group", "192.168.1.103")->
			property<Glib::ustring>("multicast-iface", "wlan0")->
			property<int>("port", 5015);
	Glib::RefPtr<Gst::AppSink> sink = Gst::AppSink::create();
	pipe_proto->add(udpsrc)->add(sink);
	udpsrc->link(sink);
	pipe_proto->set_state(Gst::STATE_PLAYING);
	constexpr unsigned long max_buf = 2048;
	unsigned char data_dump[max_buf];
static bool flip = false;
	while (true)
	{
		auto sample = sink->pull_sample();
		if (!sample)
			continue;
		auto buffer = sample->get_buffer();
		if (!buffer)
			continue;
		auto mapper = Glib::RefPtr<Gst::MapInfo>(new Gst::MapInfo());
		buffer->map(mapper, Gst::MAP_READ);
		int size = buffer->get_size();
		std::memcpy(data_dump, mapper->get_data(), size);

		for (int i = 0; i < size; i++)
		{
			std::cout << data_dump[i] << std::endl;
			if (data_dump[i] == 'h')
			{
				flip = !flip;
				rvw.horizontalFlip(flip);
				server->Stop();
				set_rvw_config();
				set_server_config();

				server->Setup();
				server->Play();
			}
		}

		// todo read from uart
		/*byte b;
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
		frame_proxy_free(&proxy);*/
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
