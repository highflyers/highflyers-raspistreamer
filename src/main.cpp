#include <parser.h>
#include <udpstreamer.h>
#include <glibmm.h>

void parse_stream_info(StreamerInfo* info)
{

}

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

void uart_acquisition()
{
	HighFlyersParser parser;
	parser_initialize(&parser);

	while (true)
	{
		parser_append_byte(&parser, 0); // todo byte

		if (parser_has_frame(&parser))
		{
			FrameProxy proxy = parser_get_last_frame_ownership(&parser);

			switch (proxy.type)
			{
			case T_StreamerInfo:
				parse_stream_info(static_cast<StreamerInfo*>(proxy.pointer));
				break;
			default:
				break;
			}

			frame_proxy_free(&proxy);
		}
	}
}

Glib::RefPtr<Glib::MainLoop> main_loop;

struct Config
{
	std::string ip;
	int port = -1;
	int width = -1;
	int height = -1;
	int framerate = -1;
	int bitrate = -1;
	bool horizontal_flip = false;
	bool vertical_flip = false;
};
#define INITIAL_CONFIG_LOADER(config_value, conf_str) \
	tmp = getCmdOption(argv, argv + argc, conf_str); \
	config_value = (tmp != nullptr) ? atoi(tmp) : -1;

Config load_initial_config(int argc, char** argv)
{
	Config config;
	char* tmp;
	INITIAL_CONFIG_LOADER(config.bitrate, "-b")
	INITIAL_CONFIG_LOADER(config.port, "--port")
	INITIAL_CONFIG_LOADER(config.width, "-w")
	INITIAL_CONFIG_LOADER(config.height, "-h")
	INITIAL_CONFIG_LOADER(config.framerate, "-fps")

	config.vertical_flip = cmdOptionExists(argv, argv + argc, "-vf");
	config.horizontal_flip = cmdOptionExists(argv, argv + argc, "-hf");

	tmp = getCmdOption(argv, argv + argc, "--ip");
	config.ip = (tmp) ? tmp : "";

	return config;
}

#define SET_IF_NOT_MINUS(VALUE, METHOD) \
	if (config.VALUE!= -1) \
		rvw.METHOD(config.VALUE);

void set_rvw_config(RaspiVidWrapper& rvw, const Config& config)
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

void set_server_config(GstUDPServer& server, const Config& config)
{
	if (!config.ip.empty())
		server.ip(config.ip);
	if (config.port != -1)
		server.port(config.port);
}

int main(int argc, char** argv)
{
	Config config = load_initial_config(argc, argv);

	RaspiVidWrapper rvw;
#if !(TEST_APP)

	set_rvw_config(rvw, config);

	if (rvw.start())
	{
		GstUDPServer server(rvw);
#else
	{
		GstUDPServer server(rvw, VideoSource::TEST);
#endif
		set_server_config(server, config);

		server.Setup();
		server.Play();
		main_loop = Glib::MainLoop::create();
		main_loop->run();
		server.Stop();

		rvw.close();
	}
	return 0;
}
