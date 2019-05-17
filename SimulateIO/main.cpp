//������ģ�����������ͷ��IO���
//ver 0.2 �Ľ����ݵĲ�������������ʹ�����������
/**
1.��¼ʱ��ռ��
2.ƽ����¼����
3.ƽ���������
4.��¼���ȱ�׼��
5.������ȱ�׼��
6.��¼����
7.����¼�����
8.����¼�����
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define tracesNumber 150   //���е�����
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

bool variate = false;    //�����¼��ķ�����ʱ�䳤�Ȳ���Ƿ�����ʱ��30%���ڣ�
bool variate_i = false;    //������̵Ĳ���Ƿ�����ʱ��30%���ڣ�
int timeCounter = 0;    //��ʱ���֮�ڵ���������
int lastTime;

typedef struct traceStats
{
    double eventRate=0;
    double eventTime=0;
    double aveEventTime=0;
    double aveIntervalTime=0;
    double oldAveEventTime=0;   //���ڵݹ�����׼��
    double oldAveIntervalTime=0;     //���ڵݹ�����׼��
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
        //ȷ������������
        EventLowerBound=(int)(random(eMinLower,eMinUpper));
        EventUpperBound=(int)(random(eMaxLower,eMaxUpper));
        IntervalLowerBound=(int)(random(iMinLower,iMinUpper));
        IntervalUpperBound=(int)(random(iMaxLower,iMaxUpper));
        variate = (int)(random(0,101))>70?true:false;       //�󲿷����в�������
        variate_i = (int)(random(0,101))>70?true:false;       //�󲿷����в�������
        //result<<0<<",";
        while(timeLeft>0)
        {
            //�����һ���¼�����

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
            //�˴�������Ǳ����¼��Ľ���ʱ��
            //result<<timeThisSection<<"--"<<actualTime<<",";
            timeLeft=(timeLeft>=timeThisSection)?timeLeft-timeThisSection:0;
            if(timeLeft<=0)
                break;
            //�����һ������ʱ��
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
            //�˴����������һ���¼��Ŀ�ʼʱ��
            //cout<<timeThisSection<<endl;

            Stats.oldAveIntervalTime=Stats.aveIntervalTime;
            Stats.intervalNumbers++;
            Stats.aveIntervalTime+=(timeThisSection-Stats.aveIntervalTime)/Stats.intervalNumbers;
            Stats.stdV_intTime+=(timeThisSection-Stats.oldAveIntervalTime)*(timeThisSection-Stats.aveIntervalTime);

            //result<<timeThisSection<<"--"<<actualTime<<",";
            timeLeft=(timeLeft>=timeThisSection)?timeLeft-timeThisSection:0;
        }

        //�����׼��
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









