#include<iostream>
#include<stdio.h>
#include<queue>
#include<string>
#include<Windows.h>
using namespace std;


#define Time_Change 2       //��תʱ����2
#define Memory 100        //�ڴ��С100
int memory_left = 100;

class PCB
{
public:
	PCB(string name, int size, int time)
	{
		_name = name;
		_size = size;
		_time = time;
		_left_time = time;
	};
	void Describe()
	{
		cout << _name << "(" << _left_time << ") ; " ;
	}
	void CName(string cname)
	{
		_name = cname;

	}
	void TimeUse()
	{
		_left_time -= Time_Change;
	}
	int GetMemory()
	{
		return _size;
	}
	string GetName()
	{
		return _name;
	}
	bool Jud()
	{
		//�ͷŷ���1������ִ�з���0
		if (_left_time <= 0)
		{
			//�������н����������ͷ�
			return 1;
		}
		else
		{
			return 0;
		}
	}

private:
	string _name;
	int _size;
	int _time;
	int _left_time;

};

queue<PCB>Ready, Blocked, Running, New;

void Traverse(queue<PCB> que)
{
	while (!que.empty())
	{
		que.front().Describe();
		que.pop();
	}
}

void AllQue(queue<PCB> readyque, queue<PCB> blockque, queue<PCB> runque, queue<PCB> newque)
{
	cout << "********************"<<endl;
	cout << "��ǰ�ڴ�ʣ�� : " << memory_left << endl;
	cout << "��������:" << endl;
	Traverse(readyque);
	cout << endl<<"���ж���:" << endl;
	Traverse(runque);
	cout << endl<<"��������:" << endl;
	Traverse(blockque);
	cout << endl << "New����:" << endl;
	Traverse(newque);
	cout << endl;
	cout << "********************"<<endl;

}

void Admit()
{
	while (!New.empty())
	{
		if (memory_left - New.front().GetMemory() >= 0)
		{

			cout << "��New���ύ" << New.front().GetName() << "����������" << endl;
			memory_left -= New.front().GetMemory();
			Ready.push(New.front());
			New.pop();
		}
		else
		{
			break;
		}
	}
	//AllQue(Ready, Blocked, Running,New)
}
void Dispatch()
{
	if (!Ready.size())
	{
		cout << "��������û�н���" << endl;
		//AllQue(Ready, Blocked, Running,New)
		return;
	}
	if (Running.size())
	{
		cout << "��������������" << endl;
		//AllQue(Ready, Blocked, Running,New)
		return;
	}
	Running.push(Ready.front());
	Ready.pop();
	//AllQue(Ready, Blocked, Running,New)

}

void Release()
{
	cout << "�ͷŽ���" << Running.front().GetName()<<endl;
	memory_left += Running.front().GetMemory();
	Running.pop();
	//�����½���
	if (!Ready.empty())
	{
		Dispatch();
	}
	Admit();
	//AllQue(Ready, Blocked, Running,New)
}

void TimeOut()
{
	//Running->Ready
	if (Running.empty())
	{
		cout << "û�������еĽ���" << endl;
		//AllQue(Ready, Blocked, Running,New)
		return;
	}
	cout << "Runing 2 s ........." << endl;
	Sleep(2000);
	PCB &p = Running.front();
	p.TimeUse();
	if (!Ready.empty())
	{
		Running.push(Ready.front());
		Ready.pop();
	}
	if (p.Jud())
	{
		cout << "Release--------";
		Release();
		return;
	}
	Ready.push(Running.front());
	Running.pop();
	//AllQue(Ready, Blocked, Running,New)
}

void EventWait()
{
	//Running->Blocked
	if (Running.empty())
	{
		cout << "û�������еĽ���" << endl;
		//AllQue(Ready, Blocked, Running,New)
		return;
	}
	Blocked.push(Running.front());
	Running.pop();
	if (Ready.size())
	{
		Running.push(Ready.front());
		Ready.pop();
	}
	//AllQue(Ready, Blocked, Running,New)
}

void EventOccur()
{
	//Blocked->Ready
	if (Blocked.empty())
	{
		cout << "û���������Ľ���" << endl;
		//AllQue(Ready, Blocked, Running,New)
		return;
	}
	Ready.push(Blocked.front());
	Blocked.pop();
	//AllQue(Ready, Blocked, Running,New)
}

void NewProcess()
{
	string name;
	int size, time;
	cout << "��������� : ";
	cin >> name;
	cout << endl;
	cout << "ռ���ڴ��С: ";
	cin >> size;
	cout << "ռ��ʱ���С: ";
	cin >> time;
	PCB p(name, size, time);
	New.push(p);
	Admit();
	
}


int main()
{
	string a ;
	int i = 3, select,exit;
	//Traverse(Ready);
	cout << "�������(�������У�������eof����" << endl;
	while (cin>>a)
	{
		if (a == "eof")
		{
			break;
		}
		int size, time;
		cout << "ռ���ڴ��С: ";
		cin >> size;
		cout << "ռ��ʱ���С: ";
		cin >> time;
		PCB p(a,size,time);
		New.push(p);
		if (memory_left-New.front().GetMemory()>=0)
		{
			Ready.push(p);
			memory_left -= size;
			New.pop();
		}
	}
	
	cout << "Running 2 s ......." << endl;
	Sleep(2000);
	cout << "Dispacth---------------" << endl;
	Dispatch();
	AllQue(Ready, Blocked, Running, New);
	cout << "(0)Creat  (1)Dispatch  "<<endl<<"(2)Timeout  (3)EventOccur " << endl
		<< "(4)EventWait  (5)exit" << endl;
	while (cin >> select)
	{
		cout<< "----------"<<endl;
		if (select == 5)break;
		switch (select)
		{
		case 0:
		{
			NewProcess();
		}
		case 1:
		{
			Dispatch();
			break;
		}
		case 2:
		{
			//Running->Ready
			TimeOut();
			break;
		}
		case 3:
		{
			//Blocked->Ready
			EventOccur();
			break;
		}
		case 4:
		{
			//Running->Blocked
			EventWait();
			break;
		}
		default:
			break;
		}
		AllQue(Ready, Blocked, Running, New);
	}

	system("pause");

}