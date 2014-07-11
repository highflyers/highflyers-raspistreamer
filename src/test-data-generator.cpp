/*
 * test-data-generator.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

extern "C" {
#include "parser.h"
}
#include <gstreamermm.h>
#include <gstreamermm/appsrc.h>
#include <iostream>
#include <cstring>
#include <glibmm.h>

using namespace Gst;
using Glib::RefPtr;
using namespace std;

int main(int argc, char** argv)
{
	Gst::init(argc, argv);

	StreamerInfo info;

	info.framerate = 1;
	info.height = 1024;
	info.width = 1024;
	info.port = 8080;
	info.ip = { 127, 0, 0, 1};


	byte output[1024];

	StreamerInfo_serialize(&info, output);

	auto pipeline = Pipeline::create();
	auto src = AppSrc::create();
	auto sink = ElementFactory::create_element("udpsink");
	sink->property("port", 5006);
	pipeline->add(src)->add(sink);
	src->link(sink);
	pipeline->set_state(STATE_PLAYING);

	HighFlyersParser parser;
	parser_initialize(&parser);

	for (int i = 0; i < 1; i++)
	{
		auto b = Buffer::create(100);
		RefPtr<MapInfo> mapper(new MapInfo());
		b->map(mapper, MAP_WRITE);
		memcpy(mapper->get_data(), output, 100);
		b->unmap(mapper);
		std::cout << src->push_buffer(b) << std::endl;
	}
	Glib::MainLoop::create()->run();

	for (int i = 0; i < 1024; i++)
	{
		parser_append_byte(&parser, output[i]);
		cout << (int)output[i] << " ";

		if (!parser_has_frame(&parser))
			continue;

		StreamerInfo* parsed = static_cast<StreamerInfo*>(parser_get_last_frame_ownership(&parser).pointer);
		cout << endl << parsed->height << " " << parsed->width << " " << parsed->port << " "
					<< (int)parsed->ip.ip1 << "." << (int)parsed->ip.ip2 << "."
					<< (int)parsed->ip.ip3 << "." << (int)parsed->ip.ip4 << endl;
		break;
	}


	return 0;
}


