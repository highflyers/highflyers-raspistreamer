/*
 * GstUDPServer.h
 *
 *  Created on: 06-05-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#ifndef GSTUDPSERVER_H_
#define GSTUDPSERVER_H_

#include <iostream>
#include <glibmm.h>
#include <gstreamermm.h>
#include "raspividwrapper.h"

class GstUDPServer
{
private:
	Glib::RefPtr<Gst::Pipeline> pipeline;
	Glib::RefPtr<Gst::Element> element_source, element_h264parse,
			element_rtph264pay, element_sink;

	std::string _ip;
	int _port;

	const RaspiVidWrapper& rvw;

	void createElements();
	void addToPipeline();
	void linkElements();

public:
	GstUDPServer(const RaspiVidWrapper& rv);

	void Setup();
	void Play();
	void Stop();

	//getters and setters
	std::string const& ip() const
	{
		return _ip;
	}
	void ip(std::string const& newIP);

	int const& port() const
	{
		return _port;
	}
	void port(int const& newPort);
};

#endif /* GSTUDPSERVER_H_ */
