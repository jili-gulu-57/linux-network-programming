#include "myshell.h"

char* gargv[ARGS]={NULL};
int gargc=0;


static std::string GetUserName()
{
    std::string user=getenv("USER");
    return user.empty()?"None":user;
}

static std::string GetHostName()
{
    std::string hostname = getenv("HOSTNAME");
    return hostname.empty()?"None": hostname;
}

static std::string GetPwd()
{
    std::string pwd = getenv("PWD");
    return pwd.empty()?"None": pwd;
}

//获取家目录
static std::string GetHomePath()
{
    std::string home=getenv("HOME");
    return home.empty() ? "/" : home;
}
// 输出提示符
void PrintCommandPrompt()
{

    std::string user = GetUserName();
    std::string hostname = GetHostName();
    std::string pwd = GetPwd();
    printf("[%s@%s %s]#",user.c_str(),hostname.c_str(),pwd.c_str());
}
//获取用户的键盘输入
bool GetCommandString(char cmd_str_buff[], int len)
{

    if(cmd_str_buff == NULL |len<= 0)
        return false;
    char *res =fgets(cmd_str_buff, len, stdin);
    if(res == NULL)
        return false;

    //ls -a\n --> ls -a\0
    cmd_str_buff[strlen(cmd_str_buff)-1]=0;
    return strlen(cmd_str_buff)==0?false:true;
}

//解析命令行
bool ParseCommandString(char cmd[])
{
    //安全性检查
    if(cmd==NULL)
        return false;

#define SEP " "
    gargc=0;
    gargv[gargc++]=strtok(cmd,SEP);

    while((bool)(gargv[gargc++] = strtok(NULL,SEP)));

    gargc--;

//#define DEBUG
#ifdef DEBUG 
    printf("gargc:%d\n",gargc);
    printf("-----------------------------------\n");
    for(int i=0;i<gargc;i++)
    {
        printf("gargv[%d]:%s\n",i,gargv[i]);
    }
    printf("-----------------------------------\n");
    for(int i=0;gargv[i];i++)
    {
        printf("gargv[%d]:%s\n",i,gargv[i]);
    }
#endif 

    return true;
}

void InitGlobal()
{
    gargc=0;
    memset(gargv,0,sizeof(gargv));
}

void ForkAndExec()
{
    pid_t id=fork();
    if(id<0)
    {
        //创建子进程失败
        perror("fork");
        return;
    }
    else if(id==0)
    {
        //子进程
        execvp(gargv[0],gargv);
        exit(0);        
    }
    else 
    {
        //父进程
        pid_t rid=waitpid(id,nullptr,0);
        if(rid>0)
        {

        }
    }
}

bool BuiltInCommandExec()
{
    std::string cmd=gargv[0];
    if(cmd=="cd")
    {
        //内建命令
        if(gargc==1)
        {
            //只有cd命令，切换到家目录
            chdir(GetHomePath().c_str());
            return true;
        }
        else if(gargc==2)
        {
            std::string target=gargv[1];
            if(target=="~")
            {
                chdir(GetHomePath().c_str());
                return true;
            }
            else 
            {
                chdir(gargv[1]);
                return true;
            }
        }

        //其他情况不对
        return false;
    }
    return false;
}
