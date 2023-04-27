#ifndef FILE_H
#define FILE_H

#include <QString>
#include "config.h"

class File
{
private:
    unsigned char FileNameSize;
    unsigned char *FileName;
    unsigned char FileProperty;
    unsigned char FileSize[4];
    unsigned char FileTime[7];

public:
    File();
    ~File();
    unsigned char getNameSize();
    void setNameSize(int size);
    unsigned char* getFileName();
    void setFileName(void* fileName);
    unsigned char getFileProperty();
    void setFileProperty(char property);
    unsigned char* getFileSize();
    void setFileSize(void* size);
    unsigned char* getFileTime();
    void setFileTime(void* filetime);
    int getIntFileSize();
};



#endif // FILE_H
