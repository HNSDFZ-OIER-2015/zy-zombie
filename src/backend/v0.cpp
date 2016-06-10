#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <string>

using namespace std;

struct message
{
    int cmd;                //操作
    int object,a,b,c,d;     
    //cmd=0:DEBUG
    
    //cmd=1:走动
    //object:走动的东西 a:走动的方向                             走动：1右 2上 3左 4下
    
    //cmd=2:开枪
    //object:开枪的东西 a:开枪的方向 b:枪支                      走动：1右 2上 3左 4下
};

struct person
{
    int kind;                       //1:玩家 2:小脏比 3:大脏比
    
    int x,y;                        //坐标:平面直角坐标系，第一象限
};

int area[100][100];

queue <message> messageQueue;          //消息队列

string getGunname(int gun)
{
    switch(gun)
    {
        case 1:return ("ShortGun");
    }
    return ("404NotFound.");
}

int msgDebug(message msg)
{
    printf("DEBUG. Get MSG: cmd=%d object[%d] a=%d b=%d c=%d d=%d .\n",msg.cmd,msg.object,msg.a,msg.b,msg.c,msg.d);
    return 0;
}

int msgWalk(message msg)
{
    printf("Walk object[%d] to %d.\n",msg.object,msg.a);
    
    return 0;
}

int msgShot(message msg)
{
    int i;
    printf("Shot object[%d] to %d , use %s.\n",msg.object,msg.a,getGunname(msg.b).c_str());
    
    if(msg.a==1) goto finishShot;           //右
    if(msg.a==2) goto finishShot;           //上
    if(msg.a==3) goto finishShot;           //左
    if(msg.a==4) goto finishShot;           //下
    
    finishShot:
    
    return 0;
}

void getMessage()
{
    message msg;
    
    while(!messageQueue.empty())
    {
        msg=messageQueue.front();
        messageQueue.pop();
        
        switch(msg.cmd)
        {
           case 0:
                msgDebug(msg);
                break;
           case 1:
                msgWalk(msg);
                break;
           case 2:
                msgShot(msg);
        }
    }
}

int main(void)
{
    message test;
    test.cmd=1,test.object=1,test.a=4;
    messageQueue.push(test);
    test.cmd=2,test.object=1,test.a=4,test.b=1;
    messageQueue.push(test);
    test.cmd=1,test.object=1,test.a=2;
    messageQueue.push(test);
    getMessage();
}