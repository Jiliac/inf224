#include <stdio.h>
#include <iostream>
#include "Group.h"

Group::~Group(){};

string Group::printGroup() {
  string toPrint;
  toPrint = "I am a group and my name is : " + group_name + " -- ";
  for (std::list<intrusive_ptr<File> >::iterator i = this->begin(); i != this->end(); ++i) {
    toPrint += (*i)->printAttr();
    toPrint += " -- ";
  }
  return toPrint;
}
  
