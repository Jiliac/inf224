#include <stdio.h>
#include <iostream>
#include "Group.h"

Group::~Group(){};

void Group::printGroup() {
  cout << "I am a group and my name is : " << group_name << "\n";
  for (std::list<intrusive_ptr<File> >::iterator i = this->begin(); i != this->end(); ++i) {
    (*i)->printAttr();
    cout << "\n";
  }
}
  
