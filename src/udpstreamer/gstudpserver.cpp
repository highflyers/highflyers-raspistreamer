/*
 * GstUDPServer.cpp
 *
 *  Created on: 06-05-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "gstudpserver.h"

void GstUDPServer::createElements()
{
	element_source = Gst::ElementFactory::create_element("fdsrc");
	element_h264parse = Gst::ElementFactory::create_element("h264parse");
	element_rtph264pay = Gst::ElementFactory::create_element("rtph264pay");
	element_sink = Gst::ElementFactory::create_element("udpsink");
}

void GstUDPServer::addToPipeline()
{
	// We must add the elements to the pipeline before linking them:
	try
	{
		pipeline->add(element_source)->add(element_h264parse)->add(
				element_rtph264pay)->add(element_sink);
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
		element_source->link(element_h264parse)->link(element_rtph264pay)->link(
				element_sink);
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
	pipeline->set_state(Gst::STATE_PLAYING);
}

void GstUDPServer::Stop()
{
	pipeline->set_state(Gst::STATE_NULL);
}

GstUDPServer::GstUDPServer(const RaspiVidWrapper& rv) :
		_port(5000), rvw(rv)
{
	Gst::init();
	pipeline = Gst::Pipeline::create("rvpipeline");

	createElements();

	element_rtph264pay->set_property("config-interval", 10);
	element_rtph264pay->set_property("pt", 96);
	element_source->set_property("fd", rvw.getVideoFileDescriptor());
}

void GstUDPServer::ip(std::string const& newIP)
{
	_ip = newIP;
	if (_ip.empty())
		_ip = (char*) "127.0.0.1";

	element_sink->set_property<Glib::ustring>("host", _ip);
}

void GstUDPServer::port(int const& newPort)
{
	_port = newPort;
	element_sink->set_property("port", _port);
}
