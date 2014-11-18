#include <iostream>
#include "Group.h"

Group::~Group(){};

void Group::PrintGroup() {
  for (std::list<intrusive_ptr<File> >::iterator i = this->begin(); i != this->end(); ++i) {
    (*i)->printAttr();
  }
}
  
