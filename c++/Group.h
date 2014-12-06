#ifndef GROUPE_H
#define GROUPE_H
#include <list>
#include "File.h"
#include "intrusive_ptr.h"

using namespace std;

class Group : public list<intrusive_ptr<File> >, public Pointable{
  private:
    string group_name;
  public:
    inline Group(string _name) {
      list<intrusive_ptr<File> >();
      group_name = _name;
    }
    ~Group();

    virtual string printGroup();

    virtual string getGroupName() const {
      return group_name;
    }
};

#endif
