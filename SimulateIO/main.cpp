//本程序模拟多个监控摄像头的IO情况
//ver 0.2 改进数据的产生，将上下限使用随机数生成
/**
1.记录时间占比
2.平均记录长度
3.平均间隔长度
4.记录长度标准差
5.间隔长度标准差
6.记录次数
7.最大事件长度
8.最短事件长度
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define tracesNumber 150   //序列的数量
#define traceLength 10000   //
#define eMinUpper 10
#define eMinLower 1
#define eMaxUpper 600
#define eMaxLower 60
#define iMinUpper 20
#define iMinLower 1
#define iMaxUpper 700
#define iMaxLower 35
#define random(x,y) ((rand()%(y-x))+x)


using namespace std;

int EventLowerBound;
int EventUpperBound;
int IntervalLowerBound;
int IntervalUpperBound;

bool variate = false;    //各次事件的发生的时间长度差别是否大（相差时间30%以内）
bool variate_i = false;    //间隔长短的差别是否大（相差时间30%以内）
int timeCounter = 0;    //总时间段之内的秒数计数
int lastTime;

typedef struct traceStats
{
    double eventRate=0;
    double eventTime=0;
    double aveEventTime=0;
    double aveIntervalTime=0;
    double oldAveEventTime=0;   //用于递归计算标准差
    double oldAveIntervalTime=0;     //用于递归计算标准差
    double stdV_eventTime=0;
    double stdV_intTime=0;
    double eventNumbers=0;
    double intervalNumbers=0;
    int maxEventLength=0;
    int minEventLength=10000;
}traceStats;

void clearStat(traceStats &stat);

int main()
{
    ofstream result;
    result.open("C:\\Users\\MartinPC\\Desktop\\TraceCreated\\event.txt");
    ofstream tracePlot;
    tracePlot.open("C:\\Users\\MartinPC\\Desktop\\TraceCreated\\plot.txt");
    tracePlot<<"header"<<endl;
    ofstream traceStatsFile;
    traceStatsFile.open("C:\\Users\\MartinPC\\Desktop\\TraceCreated\\stats.csv");
    //traceStatsFile<<"eventRate,aveEventTime,aveIntervalTime,stdV_eventTime,stdV-intTime,eventNumbers,maxEventLength,minEventLength"<<endl;
    int timeLeft;
    int timeThisSection;
    int firstEventLength;
    int firstIntervalLength;
    int actualTime;
    srand((unsigned)time(NULL));
    traceStats Stats;
    for(int trNumber=0; trNumber<tracesNumber; trNumber++)
    {
        clearStat(Stats);
        lastTime = 0;
        timeLeft = traceLength;
        timeThisSection = 0;
        firstEventLength = -1;
        firstIntervalLength = -1;
        //确定本序列属性
        EventLowerBound=(int)(random(eMinLower,eMinUpper));
        EventUpperBound=(int)(random(eMaxLower,eMaxUpper));
        IntervalLowerBound=(int)(random(iMinLower,iMinUpper));
        IntervalUpperBound=(int)(random(iMaxLower,iMaxUpper));
        variate = (int)(random(0,101))>70?true:false;       //大部分序列不会齐整
        variate_i = (int)(random(0,101))>70?true:false;       //大部分序列不会齐整
        //result<<0<<",";
        while(timeLeft>0)
        {
            //先随机一个事件长度

            timeThisSection=random(EventLowerBound,EventUpperBound);
            if(variate&&(firstEventLength>=0))
            {
                while(timeThisSection<int(firstEventLength-firstEventLength*0.3)||timeThisSection>int(firstEventLength+firstEventLength*0.3))
                    timeThisSection=random(EventLowerBound,EventUpperBound);
            }
            if(firstEventLength<0)
                firstEventLength=timeThisSection;
            actualTime=(timeLeft>=timeThisSection)?(lastTime+timeThisSection):traceLength;

            Stats.eventTime+=(timeLeft>=timeThisSection)?timeThisSection+1:timeLeft+1;
            Stats.eventNumbers++;
            Stats.oldAveEventTime=Stats.aveEventTime;
            Stats.aveEventTime+=(timeThisSection-Stats.aveEventTime)/Stats.eventNumbers;
            if(timeThisSection>Stats.maxEventLength)
                Stats.maxEventLength=timeThisSection;
            if(timeThisSection<Stats.minEventLength)
                Stats.minEventLength=timeThisSection;
            Stats.stdV_eventTime+=(timeThisSection-Stats.oldAveEventTime)*(timeThisSection-Stats.aveEventTime);

            for(int k=lastTime; k<=actualTime; k++)
                tracePlot<<k<<",";
            lastTime=actualTime;
            //cout<<timeThisSection<<endl;
            //此处输出的是本次事件的结束时间
            //result<<timeThisSection<<"--"<<actualTime<<",";
            timeLeft=(timeLeft>=timeThisSection)?timeLeft-timeThisSection:0;
            if(timeLeft<=0)
                break;
            //后随机一个空闲时间
            timeThisSection=random(IntervalLowerBound,IntervalUpperBound);
            if(variate_i&&(firstIntervalLength>=0))
            {
                while(timeThisSection<int(firstIntervalLength-firstIntervalLength*0.3)||timeThisSection>int(firstIntervalLength+firstIntervalLength*0.3))
                    timeThisSection=random(IntervalLowerBound,IntervalUpperBound);
            }
            if(firstIntervalLength<0)
                firstIntervalLength=timeThisSection;

            actualTime=(timeLeft>=timeThisSection)?(lastTime+timeThisSection):traceLength;
            lastTime=actualTime;
            //此处输出的是下一次事件的开始时间
            //cout<<timeThisSection<<endl;

            Stats.oldAveIntervalTime=Stats.aveIntervalTime;
            Stats.intervalNumbers++;
            Stats.aveIntervalTime+=(timeThisSection-Stats.aveIntervalTime)/Stats.intervalNumbers;
            Stats.stdV_intTime+=(timeThisSection-Stats.oldAveIntervalTime)*(timeThisSection-Stats.aveIntervalTime);

            //result<<timeThisSection<<"--"<<actualTime<<",";
            timeLeft=(timeLeft>=timeThisSection)?timeLeft-timeThisSection:0;
        }

        //处理标准差
        Stats.stdV_eventTime=sqrt(Stats.stdV_eventTime/Stats.eventNumbers);
        Stats.stdV_intTime=sqrt(Stats.stdV_intTime/Stats.intervalNumbers);

        Stats.eventRate=Stats.eventTime/traceLength;
        Stats.stdV_eventTime/=Stats.aveEventTime;
        Stats.stdV_intTime/=Stats.aveIntervalTime;
        Stats.eventNumbers/=traceLength;
        traceStatsFile<<Stats.eventRate<<","<<Stats.aveEventTime<<","<<Stats.aveIntervalTime<<","<<Stats.stdV_eventTime<<",";
        traceStatsFile<<Stats.stdV_intTime<<","<<Stats.eventNumbers<<","<<Stats.maxEventLength<<","<<Stats.minEventLength<<endl;

        tracePlot<<endl;
        for(int k=0; k<Stats.eventTime; k++)
            tracePlot<<trNumber+1<<",";
        result<<endl;
        tracePlot<<endl;
    }
    result.close();
    return 0;
}

void clearStat(traceStats &stat)
{
    stat.eventRate=0;
    stat.eventTime=0;
    stat.aveEventTime=0;
    stat.aveIntervalTime=0;
    stat.oldAveEventTime=0;
    stat.oldAveIntervalTime=0;
    stat.stdV_eventTime=0;
    stat.stdV_intTime=0;
    stat.eventNumbers=0;
    stat.intervalNumbers=0;
    stat.maxEventLength=0;
    stat.minEventLength=10000;
    return ;
}









