/*
 * RaspiVidWrapper.h
 *
 *  Created on: 06-05-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#ifndef RASPIVIDWRAPPER_H_
#define RASPIVIDWRAPPER_H_

#include <stdio.h>
#include <string>
#include <sstream>

class RaspiVidWrapper
{
private:
	FILE* stream;
	std::string prepareArg();

	unsigned int _width;
	unsigned int _height;
	unsigned int _bitrate;
	unsigned int _framerate;
	bool _verticalFlip;
	bool _horizontalFlip;
public:
	RaspiVidWrapper();

	/* Return the system file descriptor for video stream.  */
	int getVideoFileDescriptor() const;

	bool start();
	bool close();

	//getters and setters
	unsigned int const& width() const
	{
		return _width;
	}
	void width(unsigned int const& newWidth)
	{
		_width = newWidth;
	}

	unsigned int const& height() const
	{
		return _height;
	}
	void height(unsigned int const& newHeight)
	{
		_height = newHeight;
	}

	unsigned int const& bitrate() const
	{
		return _bitrate;
	}
	void bitrate(unsigned int const& newBitrate)
	{
		_bitrate = newBitrate;
	}

	unsigned int const& framerate() const
	{
		return _framerate;
	}
	void framerate(unsigned int const& newFramerate)
	{
		_framerate = newFramerate;
	}

	bool const& verticalFlip() const
	{
		return _verticalFlip;
	}
	void verticalFlip(bool const& flip)
	{
		_verticalFlip = flip;
	}

	bool const& horizontalFlip() const
	{
		return _horizontalFlip;
	}
	void horizontalFlip(bool const& flip)
	{
		_horizontalFlip = flip;
	}
};

#endif /* RASPIVIDWRAPPER_H_ */
