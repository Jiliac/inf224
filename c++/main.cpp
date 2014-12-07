#include <stdio.h>
#include <iostream>
#include "TCPServer.h"


int main(int argc, char** argv)
{
  TCPServer* server = new TCPServer();
  server->run(3331);

	return 0;	
}
