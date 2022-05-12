#ifndef JOB_H
#define JOB_H
#include<vector>
#include<QTextBrowser>
#include<QMainWindow>
#include<QPainter>
#define max_Frame 4  //每个进程所分配的叶框最大数
using namespace std;

class job
{
public:
    int pages_max;//最大页号
    int save_Frame[max_Frame]={-1,-1,-1,-1};//物理叶框
    vector<int> interview_Arr;//页面调度顺序
    QTextBrowser *Browser;//展示组件1
    QTextBrowser *Browser2;//展示组件2
    void clear_save_Frame();//清空叶框

public:
    job();
    void OPT();
    void FIFO();
    void LRU();
    void NUR();
    int Find_Exist(int addr);//查看页号addr是否命中
    int Find_LeastInteviewTime(int start,int m);//从第start个调度开始，找到叶框内第m块内存的页下一次调入的间隔
    void print_save_Frame();
    void write_interviewArr(QString str);
};

#endif // JOB_H
