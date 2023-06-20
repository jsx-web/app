#include "fileoperation.h"


FileOperation::FileOperation()
{
    memset(FileID,0,4);
    memset(FileOffsetPer,0,4);
    memset(FileOffset,0,4);
    FollowFlage = 0x00;
    FileDate = nullptr;
    CRC = 0x00;
}

FileOperation::~FileOperation()
{
    delete FileDate;
}

unsigned char *FileOperation::getFileID()
{
    return FileID;
}

void FileOperation::setFileID(void *fileID)
{
    my_strncpy(FileID,fileID,4);
}

unsigned char *FileOperation::getFileOffsetPer()
{
    return FileOffsetPer;
}

unsigned char *FileOperation::getFileOffset()
{
    return FileOffset;
}

void FileOperation::setFileOffset(void *fileOffset)
{
    if(getIntOffset(fileOffset)-getIntOffset(FileOffset)>0)
    {
        my_strncpy(FileOffsetPer,FileOffset,4);
        my_strncpy(FileOffset,fileOffset,4);
    }
}

unsigned char FileOperation::getFollowFlage()
{
    return FollowFlage;
}

void FileOperation::setFollowFlage(char followFlage)
{
    FollowFlage = (unsigned char)followFlage;
}

unsigned char *FileOperation::getFileDate()
{
    int len = getIntFileSize();
    if(len != 0)
    {
       return FileDate;
    }
    return 0x00;
}

void FileOperation::setFileDate(void *fileDate,char* path,int data_len)
{
    cout << "******************" << __FUNCTION__ << "******************" << endl;
    int len = getIntFileSize();
//    int offset = (getIntOffset(FileOffset) - getIntOffset(FileOffsetPer));
    int name_len = getNameSize();
    if(len != 0)
    {
        char *name = new char[name_len+1];
        memset(name,0,name_len+1);
        my_strncpy(name,getFileName(),name_len);
        cout << "name: " << name << endl;
        char *data = new char[data_len+1];
        memset(data,0,data_len+1);
        cout << "offset: " << data_len << endl;
        my_strncpy(data,fileDate,data_len);
        //show_hex(data,offset);
        writeFile(name,data,path);
    }
    cout << "**************************************" << endl;
}

unsigned char FileOperation::getCRC()
{
    return CRC;
}

void FileOperation::setCRC(char crc)
{
    //校验算法还未写
    CRC = crc;
}

void FileOperation::setFileOpSize(void *size)
{
    setFileSize(size);
    int len = getIntFileSize();
    FileDate = new unsigned char[len];
}


void FileOperation::setWFileOffset(int wfileOffset)
{
    union Date
    {
        unsigned int a;
        unsigned char arr[4];
    }data;
    data.a=wfileOffset;
    my_strncpy(WFileOffset,data.arr,4);
}

unsigned char *FileOperation::getWFileOffset()
{
    return WFileOffset;
}

int FileOperation::getIntOffset(void* fileOffset)
{
    int ret =0;
    memcpy(&ret,fileOffset,4);
    return ret;
}

bool FileOperation::ClearFile(char *Name, char *path)
{
    QString dir_str;
//    cout << "path: " << path << endl;
//    cout << "path_strlen: " << strlen(path) << endl;

    if(strlen(path) == 0)
    {
        dir_str = QString(PATH);
    }
    else
    {
        dir_str = QString(path)+QString("/");
    }

    // 创建temp-path文件夹
    // 检查目录是否存在，若不存在则新建
    dir = new QDir(dir_str);
    if(!dir->exists(dir_str))
    {
          dir->mkpath(dir_str);
    }
    char filepath[100] = {0};
    strcat(filepath,dir_str.toUtf8().data());

    QString date=QString(Name);
    QStringList list = date.split("/");
    QString end = list.last();
    strcat(filepath,end.toUtf8().data());
    file = new QFile(filepath);
    printf("filepath: %s",filepath);
    //如果txt文件路径不存在
    if(!dir->exists(filepath))
    {
        file->open(QIODevice::WriteOnly | QIODevice::Text);//创建txt
        file->close();
    }
    else
    {
        cout << "clear" << endl;
        //对文件清空
        file->open(QFile::WriteOnly|QFile::Truncate);
        //关闭文件
        file->close();
    }
    return true;
}

bool FileOperation:: writeFile(char* Name,char *data,char *path)
{
    QString dir_str;
    cout << "path: " << path << endl;
    cout << "path_strlen: " << strlen(path) << endl;

    if(strlen(path) == 0)
    {
        dir_str = QString(PATH);
    }
    else
    {
        dir_str = QString(path)+QString("/");
    }

    // 创建temp-path文件夹
    // 检查目录是否存在，若不存在则新建
    dir = new QDir(dir_str);
    if(!dir->exists(dir_str))
    {
          dir->mkpath(dir_str);
    }
    char filepath[50] = {0};
    strcat(filepath,dir_str.toUtf8().data());

    QString date=QString(Name);
    QStringList list = date.split("/");
    QString end = list.last();
    printf("Name: %s",end.toUtf8().data());
    strcat(filepath,end.toUtf8().data());
    cout << filepath << endl;
    file = new QFile(filepath);
    //如果txt文件路径不存在
    if(!dir->exists(filepath))
    {
        file->open(QIODevice::WriteOnly | QIODevice::Text);//创建txt
        file->close();
    }
    else
    {
        //对文件进行写操作
        if(!file->open(QIODevice::Append | QIODevice::Text)){
            qDebug()<<"文件打开失败";
        }
        //向文件中写入字符串
        file->write(data);
        //关闭文件
        file->close();
    }

    return true;
}

int FileOperation::readFile(QString filepath, int offset,char &continueflag,unsigned char **data, char &crc)
{
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly)) {
        int n = file.size();
        file.seek(offset);
        int len = n-offset;
        if(len <= 228)
        {
            QByteArray readate = file.read(len);
            my_strncpy(*data,readate.data(),len);
            continueflag = 0x00;
        }
        else
        {
            len = 228;
            QByteArray readate = file.read(len);
            my_strncpy(*data,readate.data(),len);
            continueflag = 0x01;
        }
        file.close();
        crc = CRC_(*data,len);;
        return offset+len;
    } else {
        qDebug() << "文件打开失败！";
        file.close();
        return -1;
    }
}
