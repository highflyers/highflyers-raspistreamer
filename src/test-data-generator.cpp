/*
 * test-data-generator.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: loganek
 */

extern "C" {
#include "parser.h"
}
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	StreamerInfo info;

	info.height = 1024;
	info.width = 1024;
	info.port = 8080;
	info.ip = { 127, 0, 0, 1};

	byte output[1024];

	StreamerInfo_serialize(&info, output);

	HighFlyersParser parser;
	parser_initialize(&parser);

	for (int i = 0; i < 1024; i++)
	{
		parser_append_byte(&parser, output[i]);
		cout << (int)output[i] << " ";

		if (!parser_has_frame(&parser))
			continue;

		StreamerInfo* parsed = static_cast<StreamerInfo*>(parser_get_last_frame_ownership(&parser).pointer);
		cout << endl << parsed->height << " " << parsed->width << " " << parsed->port << " "
					<< (int)parsed->ip.ip1 << "." << (int)parsed->ip.ip2 << "."
					<< (int)parsed->ip.ip3 << "." << (int)parsed->ip.ip4 << endl;
		break;
	}


	return 0;
}


