#include "common.h"

#include <assert.h>

int main(int argc, char* argv[])
{
	Options options;
	if (parseCommandLine(argc, argv, &options))
	{
		if (options.transmit)
		{
			transmit(options);
		}
		else if (options.receive)
		{
			receive(options);
		}
		else
		{
			assert(0);
		}
	}
}
/*
output:

server:
while true; do ./ttcp_blocking -r -p9901; done
port = 9001
accepting...
receive number = 8192
receive length = 65536

client:
./ttcp_blocking -p 9001 -t localhost
port = 9001
buffer length = 65536
number of buffers = 8192
connecting to 127.0.0.1:9001
connected
512.000 MiB in total
0.440 seconds
1164.213 MiB/s
 */


