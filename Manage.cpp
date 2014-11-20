#include <stdio.h>
#include "Manage.h"
#include "intrusive_ptr.h"

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

/****************** Remove an object from all groups and the file table **************************/
void Manage::remove(string fileName) {
  if(fileTable.count(fileName) != 0) {
    for( std::map<string, intrusive_ptr<Group> >::iterator groupIterator = groupTable.begin(); groupIterator != groupTable.end(); groupIterator++) {
      intrusive_ptr<Group> group = (groupIterator->second);

      if( this->checkNameIntoGroup(group, fileName)) {
        intrusive_ptr<File> toRemove = NULL;
        for(Group::iterator file = group->begin(); file != group->end(); file++) {
          if ( (*file)->getName() == fileName) {
            toRemove = (*file);
            break;
          }
        }
        if (toRemove != NULL) {
          group->remove(toRemove);
        }
      }
    }
  }

  fileTable.erase(fileName);
}

/******************* Search for a group or a file, return it and print it **********************/
intrusive_ptr<File> Manage::searchFile(string fileName) {
  intrusive_ptr<File> searchedFile = NULL;
  if (fileTable.count(fileName) != 0) {
    searchedFile = fileTable[fileName];
    searchedFile->printAttr(); 
  } 
  return searchedFile;
}

intrusive_ptr<Group> Manage::searchGroup(string groupName) {
  intrusive_ptr<Group> searchedGroup = NULL;
  if (groupTable.count(groupName) != 0) {
    searchedGroup = groupTable[groupName];
    searchedGroup->printGroup();
  }
  return searchedGroup;
}

/******************************** Play a multimedia file *************************************/
void Manage::play(string fileName) {
  if(fileTable.count(fileName) != 0) {
    intrusive_ptr<File> fileToBePlayed = fileTable[fileName];
    fileToBePlayed->play();
  }
}


/***************************** methodes auxiliaires **********************************/
//here we just check whether or not the file is in an specific group
bool Manage::checkNameIntoGroup(intrusive_ptr<Group> group, string fileName) {
  //variable count in the case there is several iteration of the same file into a group ...
  //... which shouldn't be the case
  int count = 0;
  for (std::list<intrusive_ptr<File> >::iterator file = group->begin(); file != group->end(); ++file) {
    string fileNameToCheck = (*file)->getName(); 
    if (fileNameToCheck == fileName) {
      ++count;
    }
  }

  if (count == 0) {
    return false;
  } else if ( count == 1 ) {
    return true;
  } else {
    printf("here I return true... But you must know there is a problem...\n");
    return true;
  }
}

void Manage::checkGroupName(string group_name) {
  int count = groupTable.count(group_name);
  if (count == 0) {
    groupTable[group_name] = new Group(group_name);
  }
}
