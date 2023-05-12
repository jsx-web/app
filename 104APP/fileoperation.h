#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include "file.h"

class FileOperation : public File
{
private:
    /* File porperty
        unsigned char FileNameSize; //文件名长度
        unsigned char *FileName;    //文件名
        unsigned char FileProperty;
        unsigned char FileSize[4];  //文件大小
        unsigned char FileTime[7];
    */
    unsigned char FileID[4];
    unsigned char FileOffsetPer[4]; //文件内容的偏移指针值前
    unsigned char FileOffset[4];    //文件内容的偏移指针值后
    unsigned char WFileOffset[4];   //写文件的偏移值
    unsigned char FollowFlage;      //后续标志
    unsigned char *FileDate;        //文件数据
    unsigned char CRC;              //校验码
    QDir *dir;
    QFile *file;

    int FileOpFileSize(void* size); //存文件
public:
    FileOperation();
    ~FileOperation();
    unsigned char* getFileID();
    void setFileID(void* fileID);
    unsigned char* getFileOffsetPer();
    unsigned char* getFileOffset();
    void setFileOffset(void* fileOffset);
    unsigned char getFollowFlage();
    void setFollowFlage(char followFlage);
    unsigned char* getFileDate();
    void setFileDate(void* fileDate,char* path = nullptr);
    unsigned char getCRC();
    void setCRC(char crc);
    void setFileOpSize(void *size);
    void setWFileOffset(int wfileOffset);
    unsigned char* getWFileOffset();
    int getIntOffset(void* offset);

    bool writeFile(char* Name,char* data,char *path = nullptr);
    static int readFile(QString filepath,int offset,char &continueflag,unsigned char **data,char &crc);
};



#endif // FILEOPERATION_H
