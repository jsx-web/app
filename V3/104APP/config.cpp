#include "config.h"

char HEX[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};


// unsigned char* 转char*
char* Uchar_to_char(char* &Dest,unsigned char* Src,int Size)
{
    for(int i = 0 ; i<Size ; i++)
    {
        sprintf(Dest+i,"%c",Src[i]);
    }
    return Dest;
}

// show_hex

void show_hex(char *data, int len)
{
    for(int i =0 ; i< len ; i++)
        printf("%02x " , (unsigned char)data[i]);
    cout << endl;
}

char *hex_to_char(char **dest, char *src)
{
    int len = (unsigned char)src[1] + 2;
    cout << "len" << len << endl;
    *dest = (char*)malloc(len*2*sizeof(char)+1);
//    cout << "4" << endl;
    memset(*dest,0,len*2+1);
    int n1 , n2;
    for (int i = 0 ; i < len*2; i+=2)
    {
        n1 = (src[i/2]&0xF0)>>4;
        n2 = src[i/2]&0x0F;
        (*dest)[i] = HEX[n1];
        (*dest)[i+1] = HEX[n2];
        //cout << (*dest)[i] << " " << (*dest)[i+1] << endl;
    }
    (*dest)[len*2] = '\0';
//    cout << endl<< "=========" <<__FUNCTION__ << "===start====" << endl;
//    cout << (*dest)  << endl;
//    cout << endl<< "=========" <<__FUNCTION__ << "===end======" << endl;
    return *dest;
}

char *hex_to_char(char **dest, char *src,int srclen)
{
    //cout << "len" << len << endl;
    int len = srclen;
    *dest = (char*)malloc(len*2*sizeof(char)+1);
//    cout << "4" << endl;
    memset(*dest,0,len*2+1);
    int n1 , n2;
    for (int i = 0 ; i < len*2; i+=2)
    {
        n1 = (src[i/2]&0xF0)>>4;
        n2 = src[i/2]&0x0F;
        (*dest)[i] = HEX[n1];
        (*dest)[i+1] = HEX[n2];
        //cout << (*dest)[i] << " " << (*dest)[i+1] << endl;
    }
    (*dest)[len*2] = '\0';
//    cout << endl<< "=========" <<__FUNCTION__ << "===start====" << endl;
//    cout << (*dest)  << endl;
//    cout << endl<< "=========" <<__FUNCTION__ << "===end======" << endl;
    return *dest;
}

char *my_strncpy(void *Dest, void *Src, int size)
{
    char* start = (char*)Dest; // 记录目标字符串起始位置
    char* src = (char*)Src;
    char* dest = (char*)Dest;
    while (size) // 拷贝字符串
    {
        size--;
        *(dest++) = *(src++);
    }
    if (size) // 当count大于src的长度时，将补充空字符
    {
        while (--size)
        {
            *dest++ = '\0';
        }
    }
    return start;
}

float hextofloat (unsigned int number)
{
    //符号位
    unsigned int sign = number >>31;
    //幂数
    int exponent = ((number >> 23) & 0xff) - 0x7F;
    //尾数
    unsigned int mantissa = number << 9;
    float value = 0;
    float mantissa2;
    value = 0.5f;
    mantissa2 = 0.0f;
    while (mantissa)
    {
        if (mantissa & 0x80000000)
            mantissa2 += value;
        mantissa <<= 1;
        value *= 0.5f;
    }
    value = (1.0f + mantissa2) * (pow (2, exponent));
    if (sign) value = -value;
    return value;
}

bool Uchar_to_cp56time2a(unsigned char *date,st_cp56time2a *time)
{
    //01（毫秒低位）02（毫秒高位）03（分钟）04（时）81（日与星期）09（月）10（年）
    //unsigned char date1[7] = {0x01 ,0x02,0x03,0x04,0x81,0x09,0x10};
    uint64_t Time;
    uint64_t tmp;
    Time &= 0x0;
    tmp &= 0x0;
    for(int i=6; i>=0 ; i--)
    {
        tmp|=date[i];
        tmp = tmp<<i*8;
        Time|=tmp;
        tmp &= 0x0;
    }
    time->Time = Time;

    return true;
}

bool cp56time2a_to_char(char (&date)[7],st_cp56time2a time)
{
    memset(date,0,7);
    printf("%llx\n",time.Time);
    for (int i = 0; i < 7; i++)
    {
        date[i] = (((time.Time)>>(i*8))&0xFF);
    }
    return true;
}

QString get_cp56time2a_string(st_cp56time2a *time)
{
    char show[100] = {0};
    sprintf(show,"20%02d-%d-%d",time->Compts.year,time->Compts.month,time->Compts.mday);
    sprintf(show,"%s %02d:%02d:%02d",show,time->Compts.hour,time->Compts.min,(time->Compts.msec)/1000);
    sprintf(show,"%s %4dms ",show,(time->Compts.msec)%1000);
    sprintf(show,"%s 星期%d VI=%d(0有效1无效)",show,time->Compts.wday,(time->Compts.month)>>7);
    cout << show <<endl;
    return QString(show);
}

uint64_t get_cp56time2a_hex(st_cp56time2a *time)
{
    return time->Time;
}

void show_str(char *buf, int len)
{
    for(int i =0 ; i< len ; i++)
        printf("%c" ,buf[i]);
    cout << endl;
}

char* str_to_char(char **dest, char *src, int srclen)
{
    //cout << "len" << len << endl;
    int len = srclen;
    *dest = (char*)malloc(len/2*sizeof(char)+1);
//    cout << "4" << endl;
    memset(*dest,0,len/2+1);
    int n;
    for(int i = 0;i<(int)(strlen(src));i+=2)
    {
        n = 0;
        if(src[i]>='0'&&src[i]<='9'){n = src[i]-'0';}
        else if(src[i]>='a'&&src[i]<='f'){n = src[i]-'a'+10;}
        else if(src[i]>='A'&&src[i]<='F'){n = src[i]-'A'+10;}
        n = n<<4;
        //printf("%c ,n = %d\t",src[i],n);
        if(src[i+1]>='0'&&src[i+1]<='9'){n += src[i+1]-'0';}
        else if(src[i+1]>='a'&&src[i+1]<='f'){n += src[i+1]-'a'+10;}
        else if(src[i+1]>='A'&&src[i+1]<='F'){n += src[i+1]-'A'+10;}
        //printf("%c ,n = %d\n",src[i+1],n);
        (*dest)[i/2] = (char)n;
    }
    (*dest)[len/2+1] = '\0';
//    cout << endl<< "=========" <<__FUNCTION__ << "===start====" << endl;
//    cout << (*dest)  << endl;
//    cout << endl<< "=========" <<__FUNCTION__ << "===end======" << endl;
    return *dest;
}

int CRC_(void *vdata, int length)
{
    unsigned char *data = new unsigned char[length];
    memset(data,0,length);
    my_strncpy(data,vdata,length);
    unsigned char crc = 0;
   for (int i = 0; i < length; i++)
   {
        crc += data[i];
   }
    return crc;
}

int GetRecvSerial(unsigned char *Recv)
{

    //判断接收帧的发送序号，就是接收序号
    unsigned char Ser1 = Recv[2];
    unsigned char Ser2 = Recv[3];
    int ret = Ser2;
    ret = (ret<<7) + (Ser1>>1);

    return ret;
}
