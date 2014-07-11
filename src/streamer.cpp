/*
* streamer.cpp
*
*  Created on: Jul 2, 2014
*      Author: loganek
*/

#include "streamer.h"
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

template<typename T, int LEN, int INET>
std::string check_addr(ifaddrs* ifa)
{
	void* tmpAddrPtr = &((T*)ifa->ifa_addr)->sin_addr;
	char addressBuffer[LEN];
	inet_ntop(INET, tmpAddrPtr, addressBuffer, LEN);
	return !strncmp("wlan", ifa->ifa_name, 4) ? addressBuffer : "";
}

std::string get_ip_address()
{
	ifaddrs * ifAddrStruct = NULL;
	ifaddrs * ifa = NULL;
	std::string ret = "";

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa ->ifa_addr->sa_family==AF_INET)
			ret = check_addr<sockaddr_in, INET_ADDRSTRLEN, AF_INET>(ifa);
		else if (ifa->ifa_addr->sa_family==AF_INET6)
			ret = check_addr<sockaddr_in, INET6_ADDRSTRLEN, AF_INET6>(ifa);

		if (!ret.empty())
			break;
	}

	if (ifAddrStruct!=NULL)
		freeifaddrs(ifAddrStruct);

	return ret.empty() ? "127.0.0.1" : ret; // todo serious problem
}

#define SET_IF_NOT_MINUS(VALUE, METHOD) \
	if (config.VALUE!= -1) \
		rvw.METHOD(config.VALUE);

Streamer::Streamer(int argc, char** argv)
: config(argc, argv)
{
	server = std::make_shared<GstUDPServer>(rvw);

	dmgr.set_stream_info_callback([this](StreamerInfo* info){
		server->Stop();
		rvw.framerate(info->framerate);
		server->Setup();
		server->Play();

	});
}

Streamer::~Streamer()
{
	if (!rvw.close())
		return; // todo log or sth.
}

void Streamer::set_rvw_config()
{
	rvw.verticalFlip(config.vertical_flip);
	rvw.horizontalFlip(config.horizontal_flip);

	SET_IF_NOT_MINUS(width, width)
	SET_IF_NOT_MINUS(height, height)
	SET_IF_NOT_MINUS(framerate, framerate)
	SET_IF_NOT_MINUS(bitrate, bitrate)

	rvw.verticalFlip(config.vertical_flip);
	rvw.horizontalFlip(config.horizontal_flip);
}

void Streamer::set_server_config()
{
	if (!config.ip.empty())
		server->ip(config.ip);
	if (config.port != -1)
		server->port(config.port);
}

void Streamer::start()
{
	server->Setup();
	set_rvw_config();
	set_server_config();
	server->Play();

	dmgr.start(get_ip_address(), config.port + 1);
	while (true);
}

void Streamer::update_stream_info(const StreamerInfo* info)
{
	config.height = info->height;
	config.width = info->height;
	config.port = info->port;
	config.ip =
			std::to_string(info->ip.ip1) + "." +
			std::to_string(info->ip.ip2) + "." +
			std::to_string(info->ip.ip3) + "." +
			std::to_string(info->ip.ip4);
}
