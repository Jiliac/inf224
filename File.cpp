#include <stdio.h>
#include <iostream>
#include "File.h"
#include <stdlib.h>

/**************** File Implementation *******************/
File::File(string _name, int _acquisition_date, string _file_name)
{
  name = _name;
  acquisition_date = _acquisition_date;
  file_name = _file_name;
}
File::~File() {
  cout << "I'm destroyed; my name was: " << name << "\n";
}

// setters en getters
string File::getName() const { return name;}
int File::getAcquisitionName() const { return acquisition_date;}
string File::getFileName() const { return file_name;}
void File::setName(string _name) { name = _name;}
void File::setAcquisitionDate(int _acquisition_date) { acquisition_date = _acquisition_date;}
void File::setFileName(string _file_name) { file_name = _file_name;}

// print attribut
void File::printAttr() {
  cout << "Name : " << name << "\n";
  cout << "Acquisition date: " << acquisition_date << "\n";
  cout << "File Name: " << file_name << "\n";
}


/***************** Video Implementation ******************/
Video::Video(string _name, int _acquisition_date, string _file_name, int _length) : File(_name, _acquisition_date, _file_name)
{
  length = _length;	
}

int Video::getLength() const { return length;}
void Video::setLength(int _length) { length = _length;}

void Video::printAttr() {
  File::printAttr();
  cout << "Video length : " << length << " s\n";
} 

void Video::play() {
  string toExecute = "mplayer ";
  toExecute += this->getFileName();
  toExecute += " &";
  system(toExecute.c_str());
}

/******************* Film Implementation ********************************/
Film::Film(string _name, int _acquisition_date, string _file_name, int _nb_chapter, int* _chapter_length) : Video(_name, _acquisition_date, _file_name, 0)
{
  this->setChapterLength(_nb_chapter, _chapter_length);
}

Film::~Film() {
  delete[] chapter_length;
}

void Film::setChapterLength(int _nb_chapter, int* _chapter_length) { 
  nb_chapter = _nb_chapter;
  chapter_length = new int [nb_chapter];
  int new_length = 0;
  for (int i = 0; i < nb_chapter; ++i)
  {
    chapter_length[i] = _chapter_length[i]; 
    new_length += chapter_length[i];
  }
  this->setLength(new_length);
}
/******** ATTENTION ***********/
/* Il faudrait que cela setLength aussi */

int* Film::getChapterLength() const { return chapter_length;}
int Film::getNbChapter() const { return nb_chapter;}

void Film::printAttr() {
  Video::printAttr();
  cout << "Chapter Number :" << nb_chapter << "\n";
  for (int i = 0; i  < nb_chapter; ++i )
  {
    cout << "Chapter n°" << i << " Length:" << chapter_length[i] << "\n";
  }
}

/*************** Picture Implementation *****************/
Picture::Picture(string _name, int _acquisition_date, string _file_name, float _lat, float _lng) : File(_name, _acquisition_date, _file_name)
{
  lat = _lat;
  lng = _lng;
}

float Picture::getLat() const { return lat;}
float Picture::getLng() const { return lng;}
void Picture::setLat(float _lat) { lat = _lat;}
void Picture::setLng(float _lng) { lng = _lng;}

void Picture::printAttr() {
  File::printAttr();
  cout << "Picture Latitude : " << lat << "\n";
  cout << "Picture Longitude : " << lng << "\n";
}

void Picture::play() {
  string toExecute = "mplayer ";
  toExecute += this->getFileName();
  toExecute += " &";
  system(toExecute.c_str());
}

