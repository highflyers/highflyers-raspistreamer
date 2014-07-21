/*
 * ArduPilotPopper.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: loganek
 */

#include "ArduPilotPopper.h"
#include <gstreamermm/appsink.h>
#include <gstreamermm.h>
#include <wiringSerial.h>

using namespace Gst;
using Glib::RefPtr;

void ArduPilotPopper::open()
{
	fd = serialOpen("/dev/ttyAMA0", 115200);

	printf ("connected to /dev/ttyAMA0. Descriptor: %d\n", fd);
}

void ArduPilotPopper::write_to_uart(unsigned char c)
{
	puts ("write to uart"); // todo debug
	serialPutchar(fd, c);
}

int ArduPilotPopper::read_from_uart()
{
	if (fd == -1)
		return -1;
	int d = serialGetchar(fd);
	printf ("reading from uart %d\n", d); // todo debug

	return d;
}

void ArduPilotPopper::stream_to_ground()
{
	RefPtr<Element> udpsink = ElementFactory::create_element("udpsink");
	udpsink->property<Glib::ustring>("host", "192.168.1.100");
	udpsink->property("port", 5009);

	RefPtr<Pad> pusher = Pad::create(PadTemplate::create("pusher_tpl", PAD_SRC, PAD_ALWAYS, Caps::create_any()));
	pusher->set_active(true);
	pusher->link(udpsink->get_static_pad("sink"));
	udpsink->set_state(STATE_PLAYING);

	while (true) // todo
	{
		int b = read_from_uart();

		if (b == -1)
			continue;
		unsigned char x = (unsigned char)b;
		RefPtr<Buffer> buf = Buffer::create(1);
		gst_buffer_fill(buf->gobj(), 0, &x, 1);
		pusher->push(buf);
	}
}

void ArduPilotPopper::write_to_quadro()
{
	RefPtr<Element> udpsrc = ElementFactory::create_element("udpsrc");
	udpsrc->property<Glib::ustring>("multicast-iface", "192.168.1.100");
	udpsrc->property("port", 5010);
	RefPtr<AppSink> sink = AppSink::create();
	RefPtr<Pipeline> pipe = Pipeline::create();
	pipe->add(udpsrc)->add(sink);
	udpsrc->link(sink);
	pipe->set_state(STATE_PLAYING);

	RefPtr<MapInfo> mapper(new MapInfo());

	while (true)
	{
		auto sample = sink->pull_sample();
		puts ("Got sample from gcs"); // todo debug
		if (!sample) continue;
		auto buf = sample->get_buffer();
		if (!buf) continue;
		buf->map(mapper, MAP_READ);

		for (size_t i = 0; i < buf->get_size(); i++)
			write_to_uart(mapper->get_data()[i]);
		buf->unmap(mapper);
	}
}

void ArduPilotPopper::run()
{
	writer = std::thread([this]{write_to_quadro();});
	streamer = std::thread([this]{stream_to_ground();});
}
