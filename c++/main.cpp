#include <stdio.h>
#include <iostream>
#include "File.h"
#include "Group.h"
#include "Manage.h"
#include "TCPServer.h"


int main(int argc, char** argv)
{
  TCPServer* server = new TCPServer();
  server->run(3331);

	return 0;	
}
