/*
 * datamanager.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: loganek
 */

#include "datamanager.h"


using namespace Gst;
using Glib::RefPtr;

void DataManager::start(const std::string& ip, int port)
{
	rec_thread = std::thread([this, ip, port]{
		RefPtr<Pipeline> pipeline = Pipeline::create();
		RefPtr<Element> src = ElementFactory::create_element("udpsrc");
		RefPtr<AppSink> sink = AppSink::create();
		src->property("port", port);
		src->property("multicast-group", ip);
		pipeline->add(src)->add(sink);
		src->link(sink);
		break_rec_thread = false;
		pipeline->set_state(STATE_PLAYING);
		parser_initialize(&parser);

		while (!break_rec_thread)
		{
			RefPtr<Sample> sample = sink->pull_sample ();
			if (!sample)
				continue;
			RefPtr<Buffer> buffer = sample->get_buffer();
			if (!buffer)
				continue;

			RefPtr<MapInfo> mapper(new MapInfo());
			buffer->map(mapper, MAP_READ);

			for (unsigned int i = 0; i < buffer->get_size(); i++)
			{
				parser_append_byte(&parser, mapper->get_data()[i]);
				if (!parser_has_frame(&parser))
					continue;

				FrameProxy proxy = parser_get_last_frame_ownership(&parser);

				switch (proxy.type)
				{
				case T_StreamerInfo:
					stream_info_callback(static_cast<StreamerInfo*>(proxy.pointer));
					break;
				default:
					break;
				}
				frame_proxy_free(&proxy);
			}
			buffer->unmap(mapper);
		}
	});
}

void DataManager::stop()
{
	break_rec_thread = true;
	rec_thread.join();
}
