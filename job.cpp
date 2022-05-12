#include "job.h"
#include<QTime>
#include<QCoreApplication>
#include<string>
#include<sstream>
#include<QString>
using namespace std;

job::job()
{

}


int job::Find_Exist(int addr)
{
    for (int i = 0; i < 4; i++)
    {
        if (save_Frame[i] == addr)
        {
            return i;
        }
    }
    return -1;
}

int job::Find_LeastInteviewTime(int start,int m){
    for(int i=start;i<interview_Arr.size();i++){
        if(interview_Arr[i]==m)
            return i-start;
        else
            return 9999;
    }
}

void job::print_save_Frame(){
    Browser->clear();
    for(int i=0;i<max_Frame;i++){
        Browser->append("---");
        Browser->append(QString::number(save_Frame[i]));
        if(i==3)  Browser->append("---");
    }
}

void job::clear_save_Frame(){
    Browser->clear();
    for(int i=0;i<max_Frame;i++){
        save_Frame[i]=-1;
    }
}

void job::OPT(){
    clear_save_Frame();
    Browser2->clear();
    int addr;
    int cnt = 0;
    int score=0;
    int cur=0;
    int fail_time=0;
    while (cur < interview_Arr.size())
        {
            addr = interview_Arr[cur];
            cur++;
            Browser2->append("第"+QString::number(cur)+"轮");
            if (cnt < max_Frame)
            {
                if (Find_Exist(addr) != -1)
                {
                    score++;
                    Browser2->append(QString::number(addr)+"被命中了");

                }
                else // 未命中，但有空间，" << "\" << addr << "\" 被装入
                {
                    fail_time++;
                    Browser2->append("未命中,"+QString::number(addr)+"被装入");
                    save_Frame[cnt++] = addr;
                    print_save_Frame();
                }
            }
            else
            {
                if (Find_Exist(addr) != -1)
                {
                    score++;
                    Browser2->append(QString::number(addr)+"被命中了");
                }
                else // 未命中且没了空间
                {
                    fail_time++;
                    int* least_Time = new int[4];
                    int max_Time = 0;
                    int index;
                    for (int i = 0; i < 4; i++)
                    {
                        least_Time[i] = Find_LeastInteviewTime(cur,save_Frame[i]);
                        if (least_Time[i] > max_Time)
                        {
                            max_Time = least_Time[i];
                            index = i;
                        }
                    }
                    Browser2->append(QString::number(addr)+"替换了"+QString::number(save_Frame[index]));
                    save_Frame[index] = addr;
                    print_save_Frame();
                    delete[] least_Time;
                }
            }
            QTime time;
            time.start();
            while(time.elapsed() < 500)             //等待时间流逝0.5秒钟
                QCoreApplication::processEvents();   //处理事件
        }
    Browser2->append("缺页次数为："+QString::number(fail_time));
    double loss_Rate=1.0*fail_time/(1.0*(fail_time+score));
    Browser2->append("缺页中断率 R="+QString::number(loss_Rate));
}

void Update_InHereTime(int* in_HereTime,int n, int ind)
{
    for (int i = 0; i < n; i++)
    {
        in_HereTime[i]++;//把四个主存块的滞留时间都加一
    }
    if (ind != -1)  //ind不等于-1，则把下标为ind的主存块的滞留时间置为0
        in_HereTime[ind] = 0;
}

void job::FIFO(){
        clear_save_Frame();
        Browser2->clear();
        int* in_HereTime = new int[4];
        for (int i = 0; i < 4; i++)
            in_HereTime[i] = 0;		// 初始化都为零

        int addr;
        int cnt = 0;
        int score = 0;
        int fail_time = 0;
        int iter = 0;
        while (iter < interview_Arr.size())
        {
            addr = interview_Arr[iter];
            iter++;
            Browser2->append("第"+QString::number(iter)+"轮");
            if (cnt < max_Frame)
            {
                if (Find_Exist(addr) != -1)
                {
                    score++;
                    Browser2->append(QString::number(addr)+"被命中了");
                    print_save_Frame();
                    Update_InHereTime(in_HereTime, cnt, -1);
                }
                else // 未命中，但有空间
                {
                    fail_time++;
                    Browser2->append("未命中，"+QString::number(addr)+"被装入");
                    save_Frame[cnt] = addr;
                    print_save_Frame();
                    Update_InHereTime(in_HereTime, cnt, cnt);
                    cnt++;
                }
            }
            else
            {
                if (Find_Exist(addr) != -1)
                {
                    score++;
                    Browser2->append(QString::number(addr)+"被命中了");
                    print_save_Frame();
                    Update_InHereTime(in_HereTime,4, -1);
                }
                else // 未命中，但没了空间
                {
                    fail_time++;
                    int max_Time = 0;
                    int index;
                    for (int i = 0; i < max_Frame; i++)
                    {
                        if (in_HereTime[i] > max_Time)
                        {
                            max_Time = in_HereTime[i];
                            index = i;
                        }
                    }
                    Browser2->append(QString::number(addr)+"替换了"+QString::number(save_Frame[index]));
                    save_Frame[index] = addr;
                    print_save_Frame();
                    int ind = Find_Exist(addr);
                    Update_InHereTime(in_HereTime, max_Frame, ind);
                }
            }
            QTime time;
            time.start();
            while(time.elapsed() < 500)             //等待时间流逝0.5秒钟
                QCoreApplication::processEvents();   //处理事件
        }
        Browser2->append("缺页次数为："+QString::number(fail_time));
        double loss_Rate=(fail_time*1.0)/(1.0*(fail_time+score));
        Browser2->append("缺页中断率为："+QString::number(loss_Rate));
        delete[]  in_HereTime;
}

int Find_LeastNotUseTime(int end, int addr, vector<int>& interview_Array)
{
    for (int i = end - 1; i >= 0; i--)
    {
        if (interview_Array[i] == addr)
        {
            return end - i;
        }
    }
    return 99999;
}

void job::LRU(){
    clear_save_Frame();
    Browser2->clear();
    int addr;
    int cnt = 0;
    int score = 0;
    int fail_time = 0;
    int iter = 0;
    while (iter < interview_Arr.size())
    {
        addr = interview_Arr[iter];
        iter++;
        Browser2->append("第"+QString::number(iter)+"轮");
        if (cnt < max_Frame)
        {
            if (Find_Exist(addr) != -1)
            {
                score++;
                Browser2->append(QString::number(addr)+"被命中了");
                print_save_Frame();
            }
            else // 未命中，但有空间
            {
                fail_time++;
                Browser2->append("未命中，"+QString::number(addr)+"被装入");
                save_Frame[cnt] = addr;
                print_save_Frame();
                cnt++;
            }
        }
        else
        {
            if (Find_Exist(addr) != -1)
            {
                score++;
                Browser2->append(QString::number(addr)+"被命中了");
                print_save_Frame();
            }
            else // 未命中，但没了空间
            {
                fail_time++;
                int* Not_UseTime = new int[max_Frame];
                int max_Time = 0;
                int index;
                for (int i = 0; i < max_Frame; i++)
                {
                    Not_UseTime[i] = Find_LeastNotUseTime(iter, save_Frame[i], interview_Arr);
                    if (Not_UseTime[i] > max_Time)
                    {
                        max_Time = Not_UseTime[i];
                        index = i;
                    }
                }
                Browser2->append(QString::number(addr)+"替换了"+QString::number(save_Frame[index]));
                save_Frame[index] = addr;
                print_save_Frame();
                delete[] Not_UseTime;
            }
        }
        QTime time;
        time.start();
        while(time.elapsed() < 500)             //等待时间流逝0.5秒钟
            QCoreApplication::processEvents();   //处理事件
    }
    Browser2->append("缺页次数为："+QString::number(fail_time));
    double loss_Rate=(fail_time*1.0)/(1.0*(fail_time+score));
    Browser2->append("缺页中断率为："+QString::number(loss_Rate));
}

void job::NUR(){
    clear_save_Frame();
    Browser2->clear();
    int fail_time=0;
    int cnt=0;
    int addr;
    int score=0;
    int *flag=new int[max_Frame];
    for(int i=0;i<max_Frame;i++){
        flag[i]=1;
    }
    int replace=0;
    int iter=0;
    while (iter < interview_Arr.size()){
        addr = interview_Arr[iter];
        iter++;
        Browser2->append("第"+QString::number(iter)+"轮");
    if (cnt < max_Frame)
    {
        if (Find_Exist(addr) != -1)
        {
            score++;
            Browser2->append(QString::number(addr)+"被命中了");
            print_save_Frame();
        }
        else // 未命中，但有空间
        {
            fail_time++;
            Browser2->append("未命中，"+QString::number(addr)+"被装入");
            save_Frame[cnt] = addr;
            print_save_Frame();
            cnt++;
        }
    }
    else{
        int index=Find_Exist(addr);
        if(index!=-1){//找到
            score++;
            flag[index]=1;
            //把访问为置为1
        }
    else{//没找到
            fail_time++;
        int i=replace;
        while(i<replace+max_Frame+1){//多循环一次
        if(flag[replace]==1){
            flag[replace]=0;
            replace=(replace+1)%max_Frame;
            i++;
        }
        else{
            Browser2->append(QString::number(addr)+"替换了"+QString::number(save_Frame[replace]));
            save_Frame[replace]=addr;
            print_save_Frame();
            flag[replace]=1;
            replace=(replace+1)%max_Frame;
            break;
        }
       }
      }
    }
    QTime time;
    time.start();
    while(time.elapsed() < 500)             //等待时间流逝0.5秒钟
        QCoreApplication::processEvents();   //处理事件
   }
    double loss_Rate=(fail_time*1.0)/(1.0*(fail_time+score));
    Browser2->append("缺页次数为："+QString::number(fail_time));
    Browser2->append("缺页中断率为："+QString::number(loss_Rate));
}

void job::write_interviewArr(QString str){
    string s=str.toStdString();
    istringstream ss(s);
    int temp;
    while(ss>>temp){
        interview_Arr.push_back(temp);
    }
}
