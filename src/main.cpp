#include "streamer.h"

int main(int argc, char** argv)
{
	Streamer streamer(argc, argv);

	streamer.start();

	return 0;
}
