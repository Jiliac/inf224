#ifndef MANAGE_H
#define MANAGE_H
#include "File.h"
#include "Group.h"
#include <map>

using namespace std;

class Manage {
  private:
    map<string, intrusive_ptr<File> > fileTable;
    map<string, intrusive_ptr<Group> > groupTable;
    virtual void checkGroupName(string group_name);
   
  public:
    virtual intrusive_ptr<Group> newGroup(string group_name);
    virtual intrusive_ptr<Picture> newPicture(string group_name, string _name, int _acquisition_date, string _file_name, float _lat, float _lng);
    virtual intrusive_ptr<Video> newVideo(string group_name, string _name, int _acquisition_date, string _file_name, int _length);
    virtual intrusive_ptr<Film> newFilm(string group_name, string _name, int _acquisition_date, string _file_name, int nb_chapter, int* chapter_length);
    virtual void remove(string name);
};

#endif
