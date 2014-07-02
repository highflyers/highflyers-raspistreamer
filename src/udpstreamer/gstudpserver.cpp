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
	ge->source = ElementFactory::create_element(
			([this]{
		switch (video_source)
		{
		case VideoSource::TEST: return "videotestsrc";
		case VideoSource::FD:
		default:
			return "fd";
		}
	})()
	);
	ge->h264parse = ElementFactory::create_element("h264parse");
	ge->rtph264pay = ElementFactory::create_element("rtph264pay");
	ge->sink = ElementFactory::create_element("udpsink");

	if (video_source == VideoSource::TEST)
		ge->x264enc = ElementFactory::create_element("x264enc");
}

void GstUDPServer::addToPipeline()
{
	// We must add the elements to the pipeline before linking them:
	try
	{
		ge->pipeline->add(ge->source)->add(ge->h264parse)->add(
				ge->rtph264pay)->add(ge->sink);
		if (video_source == VideoSource::TEST)
			ge->pipeline->add(ge->x264enc);

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
		if (video_source == VideoSource::TEST)
			ge->source->link(ge->x264enc)->link(ge->h264parse);
		else
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
	addToPipeline();
	linkElements();
}

void GstUDPServer::Play()
{
	ge->pipeline->set_state(STATE_PLAYING);
}

void GstUDPServer::Stop()
{
	ge->pipeline->set_state(STATE_NULL);
}

GstUDPServer::GstUDPServer(const RaspiVidWrapper& rv, VideoSource video_source) :
		ge(new GstUDPServer::GstElements()), // make_unique not available...
		_port(5000), rvw(rv), video_source(video_source)
{
	init();
	ge->pipeline = Pipeline::create("rvpipeline");

	createElements();

	ge->rtph264pay->set_property("config-interval", 10);
	ge->rtph264pay->set_property("pt", 96);
	if (video_source == VideoSource::FD)
		ge->source->set_property("fd", rvw.getVideoFileDescriptor());
}

void GstUDPServer::ip(std::string const& newIP)
{
	_ip = newIP;
	if (_ip.empty())
		_ip = (char*) "127.0.0.1";

	ge->sink->set_property<Glib::ustring>("host", _ip);
}

void GstUDPServer::port(int const& newPort)
{
	_port = newPort;
	ge->sink->set_property("port", _port);
}
