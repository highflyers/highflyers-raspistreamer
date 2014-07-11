/*
 * RaspiVidWrapper.cpp
 *
 *  Created on: 06-05-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "raspividwrapper.h"

RaspiVidWrapper::RaspiVidWrapper() :
		stream(NULL), _width(1920), _height(1080), _bitrate(2000000), _framerate(
				30), _verticalFlip(false), _horizontalFlip(false)
{

}

std::string RaspiVidWrapper::prepareArg()
{
	std::ostringstream oss;
#if TEST_APP
	oss << "raspivid";

	oss << " -w " << _width;
	oss << " -h " << _height;
	oss << " -b " << _bitrate;
	oss << " -fps " << _framerate;

	if (_verticalFlip)
		oss << " -vf";

	if (_horizontalFlip)
		oss << " -hf";

	oss << " -n -t 0 -o -";
#else
	oss << "gst-launch-1.0 videotestsrc ! x264enc ! fdsink";
#endif

	printf ("Run raspivid: %s", oss.str().c_str());

	return oss.str();
}

int RaspiVidWrapper::getVideoFileDescriptor() const
{
	return fileno(stream);
}

bool RaspiVidWrapper::start()
{
	const char* arg = prepareArg().c_str();
	stream = popen(arg, "r");

	return (stream != NULL);
}

bool RaspiVidWrapper::close()
{
	return (pclose(stream) != -1);
}
