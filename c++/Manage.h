#ifndef MANAGE_H
#define MANAGE_H
#include "File.h"
#include "Group.h"
#include <map>

using namespace std;

class Manage : public Pointable{
  private:
    map<string, intrusive_ptr<File> > fileTable;
    map<string, intrusive_ptr<Group> > groupTable;
    virtual void checkGroupName(string group_name);
    virtual bool checkNameIntoGroup(intrusive_ptr<Group> group, string fileName);
   
  public:
    Manage() {}

    /*
     * those "new*" method are to instantiate a new object and add it to fileTable
     * and to a new group to groupTable if necessary
     */
    virtual intrusive_ptr<Group> newGroup(string group_name);
    virtual intrusive_ptr<Picture> newPicture(string group_name, string _name, int _acquisition_date, string _file_name, float _lat, float _lng);
    virtual intrusive_ptr<Video> newVideo(string group_name, string _name, int _acquisition_date, string _file_name, int _length);
    virtual intrusive_ptr<Film> newFilm(string group_name, string _name, int _acquisition_date, string _file_name, int nb_chapter, int* chapter_length);

    /*
     * to remove a file from the tables.
     * Will remove it from all the group in groupTable if necessary
     */
    virtual void remove(string fileName);


    /*
     * search a file or a group only using is name
     * The name must be the same
     */
    virtual intrusive_ptr<File> searchFile(string fileName);
    virtual intrusive_ptr<Group> searchGroup(string groupName);

    /*
     * Find a play a multimedia File
     * Once again, the file name must me exactly the same as the one used as initializer
     */  
    virtual void play(string fileName);
};

#endif
