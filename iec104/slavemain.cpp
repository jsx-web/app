#include "slave.hpp"
using std::cout;
using std::endl;
int main(int argc,char * argv[])
{
    if(argc != 3)
    {
        cout<<("输入有误\n");
        return -1;
    }
    Iec104 m_104(argv[1],argv[2]);
    int sockfd = m_104.init_tcp();
    if(sockfd == -1)
    {
        return -1;
    }
    //等待/接受客户端的连接
    while(1)
    {
        struct sockaddr_in caddr;//定义一个网络地址结构体变量，用来保存客户端的网络地址
        socklen_t len = sizeof(caddr);
        printf("====waiting for client's request=======\n");
        int connfd = accept(sockfd,(struct sockaddr*)&caddr,&len);
        if(connfd == -1)
        {
            cout<<"accept failed:"<<endl;
            continue;
        }
        cout<<"client_ip:"<<inet_ntoa(caddr.sin_addr)<<"   client_port:"<<ntohs(caddr.sin_port)<<"  连接成功"<<endl;

        pthread_t tid1;
        pthread_t tidw;
        pthread_create(&tid1,NULL,k_session,(void *)&connfd);
        pthread_create(&tidw,NULL,Report_data,(void *)&connfd);//实时发送数据                                      //bug1双线程抢占共享资源了。。。。。
    }
    pthread_exit(NULL);
    close(sockfd);

    return 1;
}