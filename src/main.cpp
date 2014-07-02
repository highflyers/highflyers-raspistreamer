#include <glibmm.h>

#include <udpstreamer.h>
#include <algorithm>

char* getCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

Glib::RefPtr<Glib::MainLoop> main_loop;

int main(int argc, char** argv)
{
	if (cmdOptionExists(argv, argv + argc, "-?")
			|| cmdOptionExists(argv, argv + argc, "--help"))
	{
		std::cout << "--ip : Set IP address" << std::endl;
		std::cout << "--port : Set port" << std::endl << std::endl;
		std::cout << "-w : Set image width. Default 1920" << std::endl;
		std::cout << "-h : Set image height. Default 1080" << std::endl;
		std::cout << "-b : Set bitrate. Use bits per second" << std::endl;
		std::cout << "-fps : Set framerate" << std::endl;
		std::cout << "-hf : Set horizontal flip" << std::endl;
		std::cout << "-vf : Set vertical flip" << std::endl;
	}
	else
	{
		char* ip = getCmdOption(argv, argv + argc, "--ip");
		char* port = getCmdOption(argv, argv + argc, "--port");

		char* width = getCmdOption(argv, argv + argc, "-w");
		char* height = getCmdOption(argv, argv + argc, "-h");
		char* framerate = getCmdOption(argv, argv + argc, "-fps");
		char* bitrate = getCmdOption(argv, argv + argc, "-b");

		RaspiVidWrapper rvw;
#if !(TEST_APP)
		if (width)
			rvw.width(atoi(width));
		if (height)
			rvw.height(atoi(height));
		if (framerate)
			rvw.framerate(atoi(framerate));
		if (bitrate)
			rvw.bitrate(atoi(bitrate));

		rvw.verticalFlip(cmdOptionExists(argv, argv + argc, "-vf"));
		rvw.horizontalFlip(cmdOptionExists(argv, argv + argc, "-hf"));

		if (rvw.start())
		{
			GstUDPServer server(rvw);
#else
		{
			GstUDPServer server(rvw, VideoSource::TEST);
#endif
			if (ip)
				server.ip(ip);
			if (port)
				server.port(atoi(port));

			server.Setup();
			server.Play();
			main_loop = Glib::MainLoop::create();
			main_loop->run();
			server.Stop();

			rvw.close();
		}
	}
	return 0;
}
