#include <iostream>

#include <tool/kv2client-config.h>

int main (int argn, char** argv)
{ std::cout << argv[0] << " : start" << std::endl;

  std::cout << "version: " 
            << KV2STREAMER_VERSION
            << std::endl;

  return 0;
}

