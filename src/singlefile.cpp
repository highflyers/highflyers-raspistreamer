#include <gstreamermm.h>
#include <iostream>
#include <glibmm.h>
#include <cstring>

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

		system("raspivid --help");
	}
	else
	{
		char* ip = getCmdOption(argv, argv + argc, "--ip");
		char* port = getCmdOption(argv, argv + argc, "--port");

		Glib::RefPtr<Gst::Pipeline> pipeline;
		Glib::RefPtr<Gst::Element> element_source, element_h264parse,
				element_rtph264pay, element_sink;

		// Initialize Gstreamermm:
		Gst::init(argc, argv);

		std::string rvcmd = "raspivid ";

		for (int i = 1; i < argc; i++)
		{
			if ((strcmp(argv[i], "--ip") == 0)
					|| (strcmp(argv[i], "--port") == 0))
			{
				i++;
				continue;
			}
			rvcmd += argv[i];
			rvcmd += " ";
		}

		rvcmd += "-n -t 0 -o -";

		FILE* stream = popen(rvcmd.c_str(), "r");

		main_loop = Glib::MainLoop::create();

		// Create pipeline:
		pipeline = Gst::Pipeline::create("rvpipeline");

		// Create elements:
		element_source = Gst::ElementFactory::create_element("fdsrc");
		element_h264parse = Gst::ElementFactory::create_element("h264parse");
		element_rtph264pay = Gst::ElementFactory::create_element("rtph264pay");
		element_sink = Gst::ElementFactory::create_element("udpsink");

		element_rtph264pay->set_property("config-interval", 10);
		element_rtph264pay->set_property("pt", 96);

		element_source->set_property("fd", fileno(stream));

		if (!ip)
			ip = (char*) "127.0.0.1";

		std::cout << "IP:" << ip << std::endl;
		element_sink->set_property<Glib::ustring>("host", ip);

		if (port)
		{
			element_sink->set_property("port", atoi(port));
			std::cout << "Port:" << port << std::endl;
		}
		else
		{
			element_sink->set_property("port", 5000);
			std::cout << "Port: 5000" << std::endl;
		}

		// We must add the elements to the pipeline before linking them:
		try
		{
			pipeline->add(element_source)->add(element_h264parse)->add(
					element_rtph264pay)->add(element_sink);
		} catch (std::runtime_error& ex)
		{
			std::cerr << "Exception while adding: " << ex.what() << std::endl;
			return 1;
		}

		// Link the elements together:
		try
		{
			element_source->link(element_h264parse)->link(element_rtph264pay)->link(
					element_sink);
		} catch (const std::runtime_error& error)
		{
			std::cerr << "Exception while linking: " << error.what()
					<< std::endl;
		}

		std::cout << "Setting to PLAYING." << std::endl;
		pipeline->set_state(Gst::STATE_PLAYING);
		std::cout << "Running." << std::endl;
		main_loop->run();

		// Clean up nicely:
		std::cout << "Returned. Stopping playback." << std::endl;
		pipeline->set_state(Gst::STATE_NULL);
		pclose(stream);
	}
	return 0;
}
