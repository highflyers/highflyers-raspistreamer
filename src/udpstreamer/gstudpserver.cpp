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
	RefPtr<Element> element_source, element_h264parse,element_rtph264pay, element_sink;
};

GstUDPServer::~GstUDPServer()
{}

void GstUDPServer::createElements()
{
	ge->element_source = ElementFactory::create_element("fdsrc");
	ge->element_h264parse = ElementFactory::create_element("h264parse");
	ge->element_rtph264pay = ElementFactory::create_element("rtph264pay");
	ge->element_sink = ElementFactory::create_element("udpsink");
}

void GstUDPServer::addToPipeline()
{
	// We must add the elements to the pipeline before linking them:
	try
	{
		ge->pipeline->add(ge->element_source)->add(ge->element_h264parse)->add(
				ge->element_rtph264pay)->add(ge->element_sink);
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
		ge->element_source->link(ge->element_h264parse)->link(ge->element_rtph264pay)->link(
				ge->element_sink);
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

GstUDPServer::GstUDPServer(const RaspiVidWrapper& rv) :
		ge(new GstUDPServer::GstElements()), // make_unique not available...
		_port(5000), rvw(rv)
{
	init();
	ge->pipeline = Pipeline::create("rvpipeline");

	createElements();

	ge->element_rtph264pay->set_property("config-interval", 10);
	ge->element_rtph264pay->set_property("pt", 96);
	ge->element_source->set_property("fd", rvw.getVideoFileDescriptor());
}

void GstUDPServer::ip(std::string const& newIP)
{
	_ip = newIP;
	if (_ip.empty())
		_ip = (char*) "127.0.0.1";

	ge->element_sink->set_property<Glib::ustring>("host", _ip);
}

void GstUDPServer::port(int const& newPort)
{
	_port = newPort;
	ge->element_sink->set_property("port", _port);
}
