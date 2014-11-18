#ifndef GROUPE_H
#define GROUPE_H
#include <string.h>
using namespace std;
#include "File.h"
#include <list>
#include "intrusive_ptr.h"

class Group : public list<intrusive_ptr<File> >, public Pointable{
  private:
    string group_name;
  public:
    inline Group(string _name) {
      list<intrusive_ptr<File> >();
      group_name = _name;
    }
    ~Group();

    virtual void PrintGroup();

    virtual string getGroupName() const {
      return group_name;
    }
};

#endif
