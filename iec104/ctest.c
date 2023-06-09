#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include <pthread.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>


#define SERV_PORT 2404//服务器端口号
#define IP "162.168.1.15"//服务器ip地址

#define c_long 300
#define MAX_SIZE 300

int flag = 0;//全局变量 通过读工作线程赋值 用以传给发送工作函数中使用

struct Msg{//用来与服务器传输数据 成员 大小 顺序都要与服务器端一样
    int id;//自己的id
    int to_id;//发送对象的id
    char name[c_long];//用户昵称
    char passwd[c_long];//用户密码
    int select;//存放选择功能 用以switch函数
    char msg[MAX_SIZE];//存放聊天内容
    int flag;//标志位 用于函数中判断进入哪个else
    int sockfd;//连接服务器套接口
    int silent;//禁言标志位 默认为0 未打开 该代码中未使用

};
typedef struct Msg Client;


void welcome(){
	printf("*************************************************************************\n");
	printf("****************             欢迎来到聊天室             *****************\n");
	printf("*************************************************************************\n");
	printf("****************      what can I do for you ,sir        *****************\n");
	printf("******                1 注册               2 登录                  ******\n");
    printf("******                3 退出               0 管理员登录            ******\n");
	printf("*************************************************************************\n");
}

void enroll(int sockfd,int select){//注册功能
    srand((unsigned)time(NULL));
    Client client;
    client.select = select;
    client.id = rand() % 90000 + 10000;//随机分配5位id 给予用户id
    printf("请输入你的用户名\n");
    scanf("%s",client.name);
    printf("请输入你的密码\n");
    scanf("%s",client.passwd);
   // send(sockfd,writeline,strlen(writeline),0);
    send(sockfd,(void *)&client,sizeof(client),0);
    sleep(1);
}

void welcome_login(){
    printf("———————————————————————————————————————————————\n");
    printf("———————   登录成功 请选择你需要的功能   ———————\n");
    printf("———————————————————————————————————————————————\n");
    printf("———————   4 查询在线用户     5 悄悄话   ———————\n");
    printf("———————   6 群聊             7 退出     ———————\n");
    printf("———————   8 查看聊天记录     12 发送文件———————\n");
    printf("———————————————————————————————————————————————\n");
}

void welcome_login_vip(){
    printf("———————————————————————————————————————————————\n");
    printf("———————    管理员登录成功 请选择功能    ———————\n");
    printf("———————————————————————————————————————————————\n");
    printf("———————   4 查询在线用户     5 悄悄话   ———————\n");
    printf("———————   6 群聊             7 退出     ———————\n");
    printf("———————   8 查看聊天记录     9 踢人     ———————\n");
    printf("———————   10 禁言            11 解除禁言———————\n");
    printf("———————   12 发送文件                   ———————\n");
    printf("———————————————————————————————————————————————\n");
}

void login(void * arg){//普通用户登录函数
    Client client = *((Client *)arg);
    int from_fd;//文件描述符
    char filename[c_long];//存放文件名
    int read_num;//用来存放read函数读取的字符数
    while(1){
        sleep(1);
        welcome_login();
        //Client client = *((Client *)arg);
        printf("尊敬的用户 请输入你的选择\n");
        scanf("%d",&client.select);
        switch(client.select){
            case 4://发送查看在线列表请求
            {
                send(client.sockfd,(void*)&client,sizeof(client),0);
                sleep(0.5);
                break;
            }
            
            case 5://悄悄话请求
            {
                printf("请输入你想发送信息用户的ID\n");
                scanf("%d",&client.to_id);
                printf("请输入你想发送的内容:\n");
                scanf("%s",client.msg);
                send(client.sockfd,(void*)&client,sizeof(client),0);
                sleep(1);
                if(flag == 4){//没有找到用户
                    break;
                }
            }
                break;
            case 6://群发
            {
                printf("请输入你想群发的信息:\n");
                scanf("%s",client.msg);
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;
            }

            case 7://退出登录界面
            {
                strcpy(client.msg,"bye");
                send(client.sockfd,(void *)&client,sizeof(client),0);
                return;
            }

            case 8://发送查看聊天记录请求
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;

            case 12://发送文件信息
                printf("请输入你想发送文件的文件名\n");
                scanf("%s",filename);
                printf("请输入你想发送用户的ID\n");
                scanf("%d",&client.to_id);

                if((from_fd = open(filename,O_RDONLY)) == -1){//打开文件
                    printf("open text1 fail!\n");
                    exit(1);
	            }
                while(read_num = read(from_fd,client.msg,c_long)){//没读完就继续读
                    if(read_num == -1){//read函数出错
                        break;
                    }
                    else if(read_num > 0){//还没读完时 将读的内容发送给客户端
                        send(client.sockfd,(void *)&client,sizeof(client),0);
                    }
                }
                break;


        }

    }
    return;
}

void login_vip(void * arg){//管理员登录函数
    Client client = *((Client *)arg);
    int from_fd;//文件描述符
    char filename[c_long];//用来存放要发送文件名
    int read_num;//用来存放发送文件时read的字符数
    while(1){
        sleep(1);
        welcome_login_vip();//管理员登录界面函数
        printf("尊敬的管理员 请输入你的选择\n");
        scanf("%d",&client.select);
        switch(client.select){
            case 4://输出在线列表
            {
                send(client.sockfd,(void*)&client,sizeof(client),0);
                sleep(0.5);
                break;
            }
            
            case 5://悄悄话
            {
                printf("请输入你想发送信息用户的ID\n");
                scanf("%d",&client.to_id);
                printf("请输入你想发送的内容:\n");
                scanf("%s",client.msg);
                send(client.sockfd,(void*)&client,sizeof(client),0);
                sleep(1);
                if(flag == 4){//没有找到该用户 就跳出switch 继续选择
                    break;
                }
            }
                break;
            case 6://群发
            {
                printf("请输入你想群发的信息:\n");
                scanf("%s",client.msg);
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;
            }

            case 7://退出登录界面
            {
                strcpy(client.msg,"bye");
                send(client.sockfd,(void *)&client,sizeof(client),0);
                return;
            }

            case 8://查看聊天记录
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;

            case 9://踢人
                printf("请输入要踢人的id:\n");
                scanf("%d",&client.to_id);
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;

            case 10://禁言
                printf("请输入您想禁言人的ID\n");
                scanf("%d",&client.to_id);
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;

            case 11://解除禁言
                printf("请输入你想解除禁言用户的ID:\n");
                scanf("%d",&client.to_id);
                send(client.sockfd,(void *)&client,sizeof(client),0);
                break;

            case 12://发送文件信息
                printf("请输入你想发送文件的文件名\n");
                scanf("%s",filename);
                printf("请输入你想发送用户的ID\n");
                scanf("%d",&client.to_id);

                if((from_fd = open(filename,O_RDONLY)) == -1){//打开文件
                    printf("open text1 fail!\n");
                    exit(1);
	            }
                while(read_num = read(from_fd,client.msg,c_long)){//没读完就继续读
                    if(read_num == -1){//read出错
                        break;
                    }
                    else if(read_num > 0){//还没读完时 将读的内容发送给客户端
                        send(client.sockfd,(void *)&client,sizeof(client),0);
                    }
                }
                break;

        }

    }

}
void* send_msg(void * agr){//发送工作函数（读写分离）
    Client client;

    int sockfd = *((int *)agr);
    client.sockfd = sockfd;

    while(1){
        welcome();//聊天室登录注册界面
        int select;
        printf("请输入你的选择：\n");
        scanf("%d",&select);
        switch(select){
            case 0://发送管理员登录请求
                client.select = select;
                sleep(0.5);
                printf("请输入管理员账号:\n");
                scanf("%d",&client.id);
                printf("请输入管理员密码:\n");
                scanf("%s",client.passwd);

                send(sockfd,(void *)&client,sizeof(client),0);

                if(flag == 9){//账号不正确
                    break;
                }
                else if(flag == 11){//密码不正确
                    break;
                }
                else{//管理员登录成功 进入登录函数
                    sleep(1);
                    login_vip((void *)&client);//进入管理员登录界面
                    sleep(0.5);
                    break;
                }
                break;

            case 1://注册普通用户功能
                enroll(sockfd,select);
                break;
            
            case 2://普通用户登录
            {
                client.select = select;
                sleep(0.5);
                printf("请输入你的账号:\n");
                scanf("%d",&client.id);
                printf("请输入你的密码:\n");
                scanf("%s",client.passwd);

                send(sockfd,(void *)&client,sizeof(client),0);

                if(flag == 1){//未找到账号
                    break;
                }
                else if(flag == 2){//密码错误
                    break;
                }
                else{//登录成功
                    sleep(1);
                    login((void *)&client);//进入普通用户登录界面
                    sleep(0.5);
                    break;
                }
                break;      
            }
            case 3://客户端下线
            {
                client.select = select;
                strcpy(client.msg,"bye");
                send(sockfd,(void *)&client,sizeof(client),0);
                close(sockfd);
                //exit(0);
                return NULL;//使得读线程结束 join函数停止阻塞 进程结束
            }
        }
    }
    return NULL;
}

void* recv_msg(void * arg){//接收工作函数（读写分离）
    Client client;
    int sockfd = (*(int *)arg);
    while(1){
        recv(sockfd,(void *)&client,sizeof(client),0);//接收来自服务器发送的信息
        switch(client.select){
            case 0://接收管理员登录是否成功的信息
                flag = client.flag;

                if(client.flag == 1){
                    printf("管理员账号错误 请重试！\n");
                }
                else if(client.flag == 2){
                    printf("管理员密码错误 请重试！\n");
                }
                else if(client.flag == 3){
                    printf("管理员登录成功\n");
                }
                break;

            case 1://注册成功 接收服务器返回的用户id
                printf("你的账号是:%d\n",client.id);
            break;

            case 2://接收登录信息
            {
                flag = client.flag;

                if(client.flag == 1){
                    printf("账号错误 请重试！\n");
                }
                else if(client.flag == 2){
                    printf("密码错误 请重试！\n");
                }
                else if(client.flag == 3){
                    printf("登录成功\n");
                }
                break;
            }

            case 3://接收客户端下线通知
                printf("%s\n",client.msg);
                //break;
                return NULL;

            case 4://接收在线用户信息
                printf("%s\n",client.msg);
                break;

            case 5://接收悄悄话
                flag = client.flag;
                if(client.flag == 4){//没有此用户
                    printf("%s\n",client.msg);
                }
                else if(client.flag == 5){//接收别人发给我的信息
                    printf("你收到来自:%d 的信息\n",client.id);
                    printf("信息内容是:%s\n",client.msg);
                }
                else{//你已经被禁言 不能发送悄悄话
                    printf("%s\n",client.msg);
                }
                break;

            case 6://接收群发消息
            {
                flag = client.flag;
                if(client.flag == 20){
                    printf("你收到了来自:%d 的群发信息\n",client.id);
                    printf("信息内容是:%s\n",client.msg);
                    break;
                }
                else{//你已经被禁言 不能发送群发消息
                    printf("%s\n",client.msg);
                }
                break;
            }

            case 7://登录后退出登录界面
                printf("%s\n",client.msg);
                break;

            case 8://接收聊天记录
                printf("%s\n",client.msg);
                break;

            case 9://接收踢人是否成功信息
                flag = client.flag;
                if(client.flag == 7){//没有此用户
                    printf("%s\n",client.msg);
                }
                else{//接收被踢出成功的信息
                    printf("%s\n",client.msg);
                    client.select = 3;//将选择改为正常退出的3功能
                    send(sockfd,(void *)&client,sizeof(client),0);//发送给服务器 让我退出
                    break;
                }
                break;

            case 10://接收禁言后的提醒
                flag = client.flag;
                if(client.flag == 12){//没有此用户
                    printf("%s\n",client.msg);
                }
                else{//接收禁言成功通知
                    printf("%s\n",client.msg);
                }
                break;

            case 11://接收解除禁言信息
                flag = client.flag;
                if(client.flag == 14){//没有此用户
                    printf("%s\n",client.msg);
                }
                else if(client.flag == 15){//该用户没有被禁言
                    printf("%s\n",client.msg);
                } 
                else{//解除禁言成功
                    printf("%s\n",client.msg);
                }
                break;

            case 12://接收文件信息
                flag = client.flag;
                if(client.flag == 17){//没有此用户
                    printf("%s\n",client.msg);
                }
                else if(client.flag == 18){//接收文件信息成功
                    //printf("收到了来自:%d\n",client.id);
                    printf("%s\n",client.msg);
                } 
                break;
        }
    }
    return NULL;
}

int main(){
    int sockfd;
    int n;
    int opt = 1;
    pthread_t tid1;//线程描述符
    pthread_t tid2;
    struct sockaddr_in servaddr;
    struct sockaddr_in clieaddr;

    sockfd = socket(AF_INET,SOCK_STREAM,0);//创建套接口

    bzero(&servaddr,sizeof(servaddr));//初始化服务器地址
    servaddr.sin_family =  AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(SERV_PORT);

    bzero(&clieaddr,sizeof(clieaddr));//初始化客户端地址
    clieaddr.sin_family = AF_INET;
	clieaddr.sin_addr.s_addr = inet_addr(IP);
	clieaddr.sin_port = htons(SERV_PORT);

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));//允许重复绑定

    bind(sockfd,(struct sockaddr *)&clieaddr,sizeof(clieaddr));//绑定套接口

    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));//请求连接


        pthread_create(&tid1,NULL,(void *)send_msg,(void *)&sockfd);//发送线程 用来向服务器发送消息
        pthread_create(&tid2,NULL,(void *)recv_msg,(void *)&sockfd);//接收线程 用来接收服务器信息

        //pthread_join(tid1,NULL);
        pthread_join(tid2,NULL);//线程等待
        close(sockfd);
}