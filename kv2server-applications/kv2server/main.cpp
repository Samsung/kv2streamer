#include "KV2Server.h"

int main(int argc, char **argv)
{
	std::string multicastIP, filename;
	if (argc == 3)
	{
		multicastIP = argv[1];
		filename	= argv[2];
	}
	else
	{
		std::cout << "Incorrect arguments specified, please specify the multicast IP address and sensor's conversion table filename excluding extension: \n";
		std::cout << "Example: kv2server_application.exe 224.1.1.1 kv2sensor01\n";
		std::cout << "\n Press Ctrl+C to quit...";
		while(1);
		return -1;
	}

	KV2Server server(multicastIP, filename);
	server.Block();
	return 0;
}
