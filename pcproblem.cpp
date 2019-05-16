#include<iostream>
#include<queue>
#include<string>

using namespace std;

int lock_ready=0;

class PCB
{
public:
    PCB(string _name,int _size)
    {
        name=_name;
        size=_size;
    };
    PCB()
    {};
    string name;
    int size;

    void Set(string _name,int _size)
    {
        name=_name;
        size=_size;
    }
    string Describe()
    {
        cout<<name<<" ; ";
        return name;
    }
};

queue<PCB>Buffer,Wait,Ready;
PCB Pcb_now;

class Semaphore
{
public:
    int count;
    string name;
    queue<PCB>wait_deque;
    Semaphore(int cot,string _name)
    {
        count=cot;
        name=_name;
    }
};
//
Semaphore full(0,"full");
Semaphore empty(6,"empty");
Semaphore mutex(1,"mutex");

void Lock_Load()
{
    if(lock_ready==0){
    lock_ready=1;
    }
}

void Un_Lock()
{
    if(lock_ready==1)
    {
        lock_ready=0;
    }
}

void P(Semaphore &s)
{
    //不允许连续两个P操作
    // if(s.count==0 && s.name=="mutex")
    // {
    //     cout<<"错误操作，请重新输入"<<endl;
    //     return;
    // }
    --s.count;
    if(s.count<0)
    {
        //调用进程进等待队列
        s.wait_deque.push(Pcb_now);
        //block process
        Lock_Load();
    }
}

void V(Semaphore &s)
{
    Un_Lock();
    // //连续两个V操作不允许
    //     if(s.count >1 && s.name=="mutex")
    // {
    //     cout<<"错误操作，请重新输入"<<endl;
    //     return;
    // }
    ++s.count;
    if(s.count<=0)
    {
        Ready.push(s.wait_deque.front());
    }
}

//Runing，消费者取数据，从Buffer中。生产者从Ready中提交到Buffer
void Running(PCB pcb,string tag)
{
    if(tag=="P")
    {
        cout<<Ready.front().name+" 存入缓冲区"<<endl;
        Buffer.push(Ready.front());
        Ready.pop();
    }
    else
    {
        cout<<Buffer.front().name+" 已经从缓冲区里取出"<<endl;
    }
    
}
void TraverseQue(queue<PCB>Wait_s,queue<PCB>Buffer_s,queue<PCB>Ready_s)
{
    cout<<"wait_queue: "<<endl;
    while (!Wait_s.empty())
	{
		Wait_s.front().Describe();
		Wait_s.pop();
	}
    cout<<endl;
    cout<<"buffer : "<<endl;
    while (!Buffer_s.empty())
	{
		Buffer_s.front().Describe();
		Buffer_s.pop();
	}
    cout<<endl;
        cout<<"ready : "<<endl;
    while (!Buffer_s.empty())
	{
		Ready_s.front().Describe();
		Ready_s.pop();
	}
    cout<<endl;
}

void TraverseSem(Semaphore s)
{
    cout<<s.name+" 等待队列: "<<endl;
    while(!s.wait_deque.empty())
    {
        cout<<s.wait_deque.front().name<<endl;
        s.wait_deque.pop();
    }
}

int main()
{
    int it=0;int t=7;
    while(t-->0)
    {
        Pcb_now.Set(to_string(it),it*10);
        it++;
        P(empty);
        if(!lock_ready)
        {
        Ready.push(Pcb_now);
        }
//        Ready.push(Pcb_now);
        if(t>=1)
        {
        P(mutex);
        Running(Pcb_now,"P");
        V(mutex);
        }
        V(full);
    }
    
    TraverseSem(empty);
    cout<<"------------------------"<<endl;
    TraverseQue(Wait,Buffer,Ready);
    cout<<"end";
    return 0;
}