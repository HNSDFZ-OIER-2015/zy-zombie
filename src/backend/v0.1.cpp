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
    int x,y;
    //cmd=0:DEBUG
    
    //cmd=1:走动
    //object:走动的东西 a:走动的方向                             走动：1右 2上 3左 4下
    
    //cmd=2:开枪
    //object:开枪的东西 a:开枪的方向 b:枪支                      走动：1右 2上 3左 4下
};

struct person
{
    int kind,id;                       //1:玩家 2:小脏比 3:大脏比
    int x,y;                        //坐标:平面直角坐标系，第一象限
};

vector <person> area[100][100];
int gunWay[100],gunAttack[100];

queue <message> messageQueue;          //消息队列

void setGun(int gun,int sss,int aaa)            //枪支id,射程,伤害
{
    gunWay[gun]=sss;
    gunAttack[gun]=aaa;
}

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
    printf("DEBUG. Get MSG: cmd=%d object[%d] a=%d b=%d c=%d d=%d (x=%d y=%d).\n",msg.cmd,msg.object,msg.a,msg.b,msg.c,msg.d,msg.x,msg.y);
    return 0;
}

int msgWalk(message msg)
{
    printf("Walk object[%d] from (%d,%d) to [%d].\n",msg.object,msg.x,msg.y,msg.a);
    
    return 0;
}

void personShot(int x,int y,int gun)
{
    int i;
    for(i=0;i<area[x][y].size();i++) printf("Object [%d] : HP-%d\n",area[x][y][i].id,gunAttack[gun]);
}

int msgShot(message msg)
{
    int i;
    printf("Shot object[%d] from (%d,%d) to [%d] , use %s.\n",msg.object,msg.x,msg.y,msg.a,getGunname(msg.b).c_str());
    
    if(msg.a==1) 
    {
        for(i=msg.x+1;i<=msg.x+gunWay[msg.b];i++) printf("Kill (%d,%d)\n",i,msg.y),personShot(i,msg.y,msg.b);
        
        goto finishShot;           //右
    }
    if(msg.a==2) 
    {
        for(i=msg.y+1;i<=msg.y+gunWay[msg.b];i++) printf("Kill (%d,%d)\n",msg.x,i),personShot(msg.x,i,msg.b);;
        goto finishShot;           //上
    }
    if(msg.a==3) 
    {
        for(i=msg.x-1;i>=msg.x-gunWay[msg.b];i--) printf("Kill (%d,%d)\n",i,msg.y),personShot(i,msg.y,msg.b);;
        goto finishShot;           //左
    }
    if(msg.a==4) 
    {
        for(i=msg.y-1;i>=msg.y-gunWay[msg.b];i--) printf("Kill (%d,%d)\n",msg.x,i),personShot(msg.x,i,msg.b);;
        goto finishShot;           //下
    }
    
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

void setPerson(int kind,int id,int x,int y)                //kind=1:玩家 2:小脏比 3:大脏比
{
    person p;
    p.id=id;
    p.kind=kind;
    p.x=x,p.y=y;
    area[x][y].push_back(p);
}

int main(void)
{
    message msg;
        
    char cmd;
    int a,b,c,d,e;
    while(~scanf("%c",&cmd))
        switch(cmd)
        {
            case 'P' : scanf("%d%d%d%d",&a,&b,&c,&d),setPerson(a,b,c,d);break;            //种类,人物id,坐标x,坐标y
            case 'G' : scanf("%d%d%d",&a,&b,&c),setGun(a,b,c);break;                    //枪支id,射程,伤害
            case 'W' : scanf("%d%d%d%d",&a,&b,&c,&d),msg.cmd=1,                        
                                                  msg.object=a,msg.x=b,msg.y=c,msg.a=d,messageQueue.push(msg);break;   
                                                  //走动的东西,坐标x,坐标y,走动的方向
            case 'A' : scanf("%d%d%d%d%d",&a,&b,&c,&d,&e),msg.cmd=2,
                                                  msg.object=a,msg.a=b,msg.b=c,msg.x=d,msg.y=e,messageQueue.push(msg);break;
                                                  //object:开枪的东西 a:开枪的方向 b:枪支
            case 'D' : getMessage();break;
            
        }
    
}