#include<iostream>
#include<queue>

using namespace std;

class PCB
{
public:
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

void Lock()
{

}

void P(Semaphore &s)
{
    if(s.count==0)
    {
        cout<<"错误操作，请重新输入"<<endl;
        return;
    }
    --s.count;
    if(s.count<0)
    {
        //调用进程进等待队列
        Wait.push(Pcb_now);
        //block process
        Lock();
    }
}

void V(Semaphore &s)
{
    ++s.count;
    if(s.count>=0)
    {
        if(s.name=="mutex")
        {
            cout<<"取消互斥"<<endl;
        }
        Buffer.push(Wait.front());
    }
}

void TraverseAll(queue<PCB>Wait_s,queue<PCB>Buffer_s,queue<PCB>Ready_s)
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

int main()
{
    Pcb_now.Set("helo",5);

    //创建5个进程
    for(int i=0;i<6,i++)
    {
        PCB p(to_string(i),i*10);
    }
    P(empty);
    P(mutex);
    Buffer.push(Pcb_now);

    cout<<"-------------------------------"<<endl;
    P(mutex);
    V(mutex);
    TraverseAll(Wait,Buffer,Ready);

}