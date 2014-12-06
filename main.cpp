#include <stdio.h>
#include <iostream>
#include "File.h"
#include "Group.h"
#include "Manage.h"
#include "TCPServer.h"


int main(int argc, char** argv)
{
  /*
  int chapter_length_ex [3] = {1, 2, 3};
	intrusive_ptr<Film> video = new Film("first_video", 1234, "video.mp4", 300, chapter_length_ex);
	intrusive_ptr<Picture> picture = new Picture("first_picture", 12345, "picture.jpg", 0,0);
  
  intrusive_ptr<Group> gr = new Group("firstgroupe");
  gr->push_back(video);
  gr->push_back(picture);
  */

  intrusive_ptr<Manage> manager = new Manage();
  int chapter_length_ex [3] = {1, 2, 3};
  manager->newFilm("group toto", "first_video", 1234, "video.mp4", 3, chapter_length_ex);

  TCPServer* server = new TCPServer();
  server->run(3331);

	return 0;	
}
