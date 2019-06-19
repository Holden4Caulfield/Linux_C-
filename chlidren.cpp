#include <iostream>
#include<unistd.h>
#include<string>
#include<deque>
#include<stdlib.h>
using namespace std;

#define SPACE 3

int main ()
{
    deque<int>series;
    for( int i = 0; i < 10; i++ )
   {
       int j;
      // 产生100以内的随机数
      j= rand()%7;
      cout<<j;
      series.push_back(j);
   }
   cout<<endl<<"----------------"<<endl;
    int a=5;
    int fd[2];
    int n;
    char buffer[100] = "main function!";
    pid_t pid1,pid2,pid3;   //进程标识符
    pipe(fd);
    pid1=fork();
    if(pid1==0)
    {
        
        // a=6;
        // cout<<"a = "<<a<<endl;
        while(!series.empty())
        {
            cout<<series.front();
            series.pop_front();
        }
        exit(0);
    }


    return 0;      //返回
}