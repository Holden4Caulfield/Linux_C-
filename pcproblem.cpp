#include<iostream>
#include<queue>
#include<string>

using namespace std;

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

void Lock()
{

}

void P(Semaphore &s)
{
    //不允许连续两个P操作
    if(s.count==0 && s.name=="mutex")
    {
        cout<<"错误操作，请重新输入"<<endl;
        return;
    }
    --s.count;
    if(s.count<0)
    {
        //调用进程进等待队列
        s.wait_deque.push(Pcb_now);
        //block process
        Lock();
    }
}

void V(Semaphore &s)
{
    //连续两个V操作不允许
    if(s.count >1 && s.name=="mutex")
    {
        cout<<"错误操作，请重新输入"<<endl;
        return;
    }
    ++s.count;
    if(s.count<=0)
    {
        Ready.push(s.wait_deque.front());
    }
}
void Running(PCB &pcb,string tag)
{
    if(tag=="P")
    {
        Buffer.push(pcb);
    }
    else
    {
        Buffer.pop();
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
    int it=0;
    Pcb_now.Set("helo",5);
    //empty.count=0;
    //创建5个进程
    // for(int i=0;i<6;i++)
    // {
    //     PCB p(to_string(i),i);
    // }
    // P(empty);
    // P(mutex);
    // Buffer.push(Pcb_now);

    // cout<<"-------------------------------"<<endl;
    // P(mutex);
    // V(mutex);
    cout<<"输入P(生产)/C(消费）,输入eof结束"<<endl;
    string tag;
    while(cin>>tag)
    {
        if(tag=="eof")break;
        //创建一个资源
        PCB p(to_string(it),it++);
        if(tag=="P")
        {
            P(empty);
            cout<<"输入P(生产)/C(消费)"<<endl;
            string tag2;
            cin>>tag2;
            if(tag2=="P")
            {
                P(mutex);
                Running(p,"P");
                V(mutex);
            }
            else if (tag2=="C")
            {
                P(mutex);
                Running(p,"C");
                V(mutex);
            }    
        }
        else if (tag=="C")
        {
            P(full);
            cout<<"输入P(生产)/C(消费)"<<endl;
            string tag2;
            cin>>tag2;
            if(tag2=="P")
            {
                P(mutex);
                Running(p,"P");
                V(mutex);
            }
            else if (tag2=="C")
            {
                P(mutex);
                Running(p,"C");
                V(mutex);
            }    
        }
        


        cout<<"输入P(生产)/C(消费）,输入eof结束"<<endl;    
    }
    TraverseAll(Wait,Buffer,Ready);
    cout<<"end";
}