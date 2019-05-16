#include<iostream>
#include<queue>
#include<string>

using namespace std;

int lock_ready=0;

//等于0表示是第一个P操作，1表示第二个P操作
int p_t=0;
int c_t=0;

//作为自动累加生成进程数据
int iter=0,iter_c=100;

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
Semaphore empty(3,"empty");
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
    while (!Ready_s.empty())
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
        TraverseSem(s);
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
        s.wait_deque.pop();
    }
    else
    {
        //开锁
        Un_Lock();
    }
    
}




//Runing，消费者取数据，从Buffer中。生产者从Ready中提交到Buffer
void Running(PCB pcb,string tag)
{
    if(tag=="P")
    {
        if(Buffer.size()==3)
        {
            cout<<"缓冲区已满，请重新操作"<<endl;
            return;
        }
        if(Buffer.front().size<100)
        {        
            cout<<Ready.front().name+" 存入缓冲区"<<endl;
            Buffer.push(Ready.front());
            Ready.pop();
        }
        else{
            cout<<"请完成消费者操作"<<endl;
        }

        TraverseQue(Wait,Buffer,Ready);
    }
    else
    {
        if(Buffer.empty())
        {
            cout<<"缓冲区无数据可以读，请重新操作"<<endl;
            return;
        }
        //就绪队列中是生产者进程
        if(Buffer.front().size<100)
        {
            cout<<Buffer.front().name+" 已经从缓冲区里取出"<<endl;
            Buffer.pop();
        }
        if(Buffer.front().size>100)
        {
            cout<<"消费者进程 "<<Buffer.front().name<<" 已完成"<<endl;
            Buffer.pop();
        }
    }
    
}
int main()
{
    int t=7;
    string x;
    cout<<"输入P(消费者),C(生产者),EOF结束,OUT"<<endl;
    while(cin>>x)
    {
        system("clear");
        cout<<"P : "<<p_t<<"      C : "<<c_t<<endl;
        cout<<"输入P(消费者),C(生产者),eof结束"<<endl;
        int select;
        if(x=="EOF")break;
        if(x=="P")
        {select=1;}
        else if (x=="C")
        {
            select=0;
        }
        else if (x=="OUT")
        {
            select=2;
            TraverseSem(full);
            TraverseSem(empty);
            TraverseQue(Wait,Buffer,Ready);
        }
        
        

        
        switch(select)
        {
        case 1:
        {
            if(p_t==0)
            {
                Pcb_now.Set(to_string(iter),iter*10);
                iter++;
                P(empty);
                if(!lock_ready)
                {
                Ready.push(Pcb_now);
                }
                p_t=1;
            }
            else
            {
                p_t=0;
                if(Buffer.size()<3)
                {
                P(mutex);
                Running(Pcb_now,"P");
                V(mutex);
                V(full);
                
                }
            cout<<"生产进程完成"<<endl;
            }
            
            break;
        }
        case 0:
        {
            { 
            Pcb_now.Set(to_string(iter_c),iter_c*10);
            iter_c++;
            }           
            if(c_t==0)
            {

                c_t=1;
                P(full);
            }
            else
            {
                c_t=0;
                P(mutex);
                Running(Pcb_now,"C");
                V(mutex);
                V(empty); 
                cout<<"消费者进程完成"<<endl;
            }
            break;
        }
        default:
            break;
        }

    cout<<endl<<"------------------"<<endl;
    }
    TraverseSem(empty);
    cout<<"------------------------"<<endl;
    TraverseQue(Wait,Buffer,Ready);

    cout<<"end";
    return 0;
}