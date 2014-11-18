#include "Manage.h"
#include "intrusive_ptr.h"

/****************** Remove an object from all all groups and the file table **************************/
void Manage::remove(string fileName) {
// TO COMPLETE
}

/*************************** Create and Add file or group to Tables **********************************/
intrusive_ptr<Group> Manage::newGroup(string group_name) {
  intrusive_ptr<Group> newGroup = new Group(group_name);
  groupTable[group_name] = newGroup;
  return newGroup; 
}

intrusive_ptr<Picture> Manage::newPicture(string group_name, string _name, int _acquisition_date, string _file_name, float _lat, float _lng) {
  intrusive_ptr<Picture> newPicture = new Picture(_name, _acquisition_date, _file_name, _lat, _lng);

  checkGroupName(group_name);

  intrusive_ptr<Group> groupeToPushInto = groupTable[group_name];
  groupeToPushInto->push_back(newPicture);

  fileTable[_name] = newPicture;

  return newPicture;
}

intrusive_ptr<Video> Manage::newVideo(string group_name, string _name, int _acquisition_date, string _file_name, int _length) {
  intrusive_ptr<Video> newVideo = new Video(_name, _acquisition_date, _file_name, _length);

  checkGroupName(group_name);

  intrusive_ptr<Group> groupeToPushInto = groupTable[group_name];
  groupeToPushInto->push_back(newVideo);

  fileTable[_name] = newVideo;

  return newVideo;
}

intrusive_ptr<Film> Manage::newFilm(string group_name, string _name, int _acquisition_date, string _file_name, int _nb_chapter, int* _chapter_length) {
  intrusive_ptr<Film> newFilm = new Film(_name, _acquisition_date, _file_name, _nb_chapter, _chapter_length);

  checkGroupName(group_name);

  intrusive_ptr<Group> groupeToPushInto = groupTable[group_name];
  groupeToPushInto->push_back(newFilm);

  fileTable[_name] = newFilm;

  return newFilm;
}


void Manage::checkGroupName(string group_name) {
  int count = groupTable.count(group_name);
  if (count == 0) {
    groupTable[group_name] = new Group(group_name);
  }
}
