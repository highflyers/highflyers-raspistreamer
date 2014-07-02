/*
 * GstUDPServer.h
 *
 *  Created on: 06-05-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#ifndef GSTUDPSERVER_H_
#define GSTUDPSERVER_H_

#include <iostream>
#include <memory>
#include "raspividwrapper.h"

class GstUDPServer
{
private:
	struct GstElements;

	std::unique_ptr<GstElements> ge;

	std::string _ip;
	int _port;

	const RaspiVidWrapper& rvw;

	void createElements();
	void addToPipeline();
	void linkElements();

public:
	GstUDPServer(const RaspiVidWrapper& rv);
	// necessary because of pImpl and unique_ptr usage
	virtual ~GstUDPServer();

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
