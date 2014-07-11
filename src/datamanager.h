/*
 * datamanager.h
 *
 *  Created on: Jul 11, 2014
 *      Author: loganek
 */

#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include "config.h"
extern "C" { // todo it should be moved to a library
#include <parser.h>
}

#include <gstreamermm.h>
#include <gstreamermm/appsink.h>

#include <functional>
#include <thread>

class DataManager
{
	std::function<void(StreamerInfo*)> stream_info_callback;
	std::thread rec_thread;
	HighFlyersParser parser;

	bool break_rec_thread;
public:
	void start(const std::string& ip, int port);
	void stop();

	void set_stream_info_callback(std::function<void(StreamerInfo*)> sic)
	{
		stream_info_callback = sic;
	}
};

#endif /* DATAMANAGER_H_ */
