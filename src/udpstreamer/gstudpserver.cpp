/*
 * GstUDPServer.cpp
 *
 *  Created on: 06-05-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "gstudpserver.h"
#include <gstreamermm.h>

using namespace Gst;
using Glib::RefPtr;

struct GstUDPServer::GstElements
{
	RefPtr<Pipeline> pipeline;
	RefPtr<Element>
		source,
		x264enc,
		h264parse,
		rtph264pay,
		sink;
};

GstUDPServer::~GstUDPServer()
{}

void GstUDPServer::createElements()
{
	ge->pipeline = Pipeline::create();
	ge->source = ElementFactory::create_element("fdsrc");
	ge->h264parse = ElementFactory::create_element(
#if !(TEST_APP)
			"h264parse"
#else
			"identity"
#endif
	);
	ge->rtph264pay = ElementFactory::create_element(
#if !(TEST_APP)
			"rtph264pay"
#else
			"identity"
#endif
	);
	ge->sink = ElementFactory::create_element("udpsink");
}

void GstUDPServer::addToPipeline()
{
	// We must add the elements to the pipeline before linking them:
	try
	{
		ge->pipeline->add(ge->source)->add(ge->h264parse)->add(
				ge->rtph264pay)->add(ge->sink);

	} catch (std::runtime_error& ex)
	{
		std::cerr << "Exception while adding: " << ex.what() << std::endl;
	}
}

void GstUDPServer::linkElements()
{
	// Link the elements together:
	try
	{
		ge->source->link(ge->h264parse);
		ge->h264parse->link(ge->rtph264pay)->link(
				ge->sink);
	} catch (const std::runtime_error& error)
	{
		std::cerr << "Exception while linking: " << error.what() << std::endl;
	}
}

void GstUDPServer::Setup()
{
	createElements();
	addToPipeline();
	linkElements();
}

void GstUDPServer::Play()
{
	if (!rvw.start())
		return; // TODO

	ge->source->set_property("fd", rvw.getVideoFileDescriptor());
	ge->sink->set_property<Glib::ustring>("host", _ip);
	ge->sink->set_property("port", _port);
#if !(TEST_APP)
	ge->rtph264pay->set_property("config-interval", 10);
	ge->rtph264pay->set_property("pt", 96);
#endif

	ge->pipeline->set_state(STATE_PLAYING);
}

void GstUDPServer::Stop()
{
	if (!rvw.close())
		return; // todo
	ge->pipeline->set_state(STATE_NULL);
	ge->pipeline.reset();
}

GstUDPServer::GstUDPServer(RaspiVidWrapper& rv) :
		ge(new GstUDPServer::GstElements()), // make_unique not available...
		_port(5000), rvw(rv)
{
	init();
}

void GstUDPServer::ip(std::string const& newIP)
{
	_ip = newIP;
	if (_ip.empty())
		_ip = (char*) "127.0.0.1";
}

void GstUDPServer::port(int const& newPort)
{
	_port = newPort;
}
