#include "file.h"
#include <iostream>
using namespace std;


File::File()
{
    FileNameSize = 0x00;
    FileName = nullptr;
    FileProperty = 0x00;
    memset(FileSize,0,4);
    memset(FileTime,0,7);
    //std::cout << "create file!!!" << std::endl;
}

File::~File()
{
    delete FileName;
}

unsigned char File::getNameSize()
{
    return FileNameSize;
}

void File::setNameSize(int size)
{
    FileNameSize = (unsigned char)(size);
    //printf("FileNameSize: %02x" , FileNameSize) ;
}

unsigned char *File::getFileName()
{
    return FileName;
}

void File::setFileName(void *fileName)
{

    if(this->getNameSize() != 0x00)
    {   
        FileName = new unsigned char[this->getNameSize()];
        memset(FileName,0,this->getNameSize());
        my_strncpy(FileName,fileName,FileNameSize);
    }
}

unsigned char File::getFileProperty()
{
    return FileProperty;
}

void File::setFileProperty(char property)
{
    FileProperty = (unsigned char)property;
}

unsigned char *File::getFileSize()
{
    return FileSize;
}

void File::setFileSize(void *size)
{
    my_strncpy(FileSize,size,4);
}

unsigned char *File::getFileTime()
{
    return FileTime;
}

void File::setFileTime(void *filetime)
{
    my_strncpy(FileTime,filetime,4);
}

int File::getIntFileSize()
{
    int ret =0;
    memcpy(&ret,FileSize,4);
    return ret;
}

void File::fileopen(char *FileName)
{

    QFileInfo file(FileName);
    //文件名称
    QString _filename = file.fileName();
    qDebug() << "_filename: " << _filename << endl;

    //文件大小，字节为单位
    qint64 size = file.size();
    int kb = size / 1024;
    cout << "kb: " << kb << endl;

//    //文件创建时间
//    QDateTime create = file.created();
//    QString _createTime =create.toString("yyyy-MM-dd hh:mm:ss");

    //文件的绝对路径
    QString _aPath =file.absoluteFilePath();
    qDebug() << "_aPath: " << _aPath << endl;

}

