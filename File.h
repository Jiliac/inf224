#ifndef FILE_H
#define FILE_H
#include <string.h>
#include "intrusive_ptr.h"

using namespace std;

/*************** Super Class **************/
class File : public Pointable
{
  private:
    string name;
    int acquisition_date;
    string file_name;

  public:
    File(string _name, int _acquisition_date, string _file_name);
    ~File();

    // setters and getters
    virtual string getName() const;
    virtual int getAcquisitionName() const;
    virtual string getFileName() const;
    virtual void setName(string);
    virtual void setAcquisitionDate(int);
    virtual void setFileName(string);

    //print atributs
    virtual void printAttr();
    
    // In all descendant class, will play the media content
    virtual void play() = 0;
};

/************** Video Classes ************/
class Video : public File
{
  private:
    int length;
  public:
    Video(string _name, int _acquisition_date, string _file_name, int _length);

    virtual int getLength() const;
    virtual void setLength(int);
    virtual void printAttr();
    virtual void play();
};

class Film : public Video
{
  private:
    int nb_chapter;
    int* chapter_length; 
  public:
    Film(string _name, int _acquisition_date, string _file_name, int _nb_chapter, int* _chapter_length);
    
    ~Film();

    virtual int* getChapterLength() const;
    virtual void setChapterLength(int, int*);
    virtual int getNbChapter() const;
    virtual void printAttr();

};

/************ Picture Classes *************/
class Picture : public File
{
  private:
    float lat;
    float lng;
  public:
    Picture(string _name, int _acquisition_date, string _file_name, float _lat, float _lng);

    virtual void printAttr();

    virtual float getLat() const;
    virtual float getLng() const;
    virtual void setLat(float);
    virtual void setLng(float);
    virtual void play();
};

#endif
