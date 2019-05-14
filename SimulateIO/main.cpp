//������ģ�����������ͷ��IO���
//ver 0.1 ���ͳ����
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

#define tracesNumber 300   //���е�����
#define traceLength 10000   //
#define shortEventLowerBound 1
#define shortEventUpperBound 10
#define longEventLowerBound 11
#define longEventUpperBound 180
#define shortIntervalLowerBound 5
#define shortIntervalUpperBound 25
#define longIntervalLowerBound 26
#define longIntervalUpperBound 600
#define random(x,y) ((rand()%(y-x))+x)


using namespace std;

int longEvent = 0;    //ÿ�η����¼�������ʱ���Ƿ� 0���� 1����� 2����
int longInterval = 0;    //û����ʱ��֮��ļ���Ƿ�  0���� 1����� 2����
bool variate = false;    //�����¼��ķ�����ʱ�䳤�Ȳ���Ƿ�����ʱ��30%��
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
    traceStatsFile.open("C:\\Users\\MartinPC\\Desktop\\TraceCreated\\stats.txt");
    traceStatsFile<<"eventRate,aveEventTime,aveIntervalTime,stdV_eventTime,stdV-intTime,eventNumbers,maxEventLength,minEventLength"<<endl;
    int timeLeft;
    int timeThisSection;
    int firstEventLength;
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
        //ȷ������������
        longEvent = (int)(random(0,30)/10);
        longInterval = (int)(random(0,30)/10);
        if(longEvent==1)
            variate=0;
        else
            variate = (int)(random(0,101))>50?true:false;
        result<<longEvent<<longInterval<<variate<<endl;
        //result<<0<<",";
        while(timeLeft>0)
        {
            //�����һ���¼�����
            switch(longEvent)
            {
            case 0:
                timeThisSection=random(shortEventLowerBound,shortEventUpperBound);
                break;
            case 1:
                timeThisSection=random(shortEventLowerBound,longEventUpperBound);
                break;
            case 2:
                timeThisSection=random(longEventLowerBound,longEventUpperBound);
                break;
            default:
                break;
            }
            if(variate&&(firstEventLength>=0))
            {
                while(timeThisSection<int(firstEventLength-firstEventLength*0.3)||timeThisSection>int(firstEventLength+firstEventLength*0.3+1))
                {
                    switch(longEvent)
                    {
                    case 0:
                        timeThisSection=random(shortEventLowerBound,shortEventUpperBound);
                        break;
                    case 1:
                        timeThisSection=random(shortEventLowerBound,longEventUpperBound);
                        break;
                    case 2:
                        timeThisSection=random(longEventLowerBound,longEventUpperBound);
                        break;
                    default:
                        break;
                    }
                }
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
            switch(longInterval)
            {
            case 0:
                timeThisSection=random(shortIntervalLowerBound,shortIntervalUpperBound);
                break;
            case 1:
                timeThisSection=random(shortIntervalLowerBound,longIntervalUpperBound);
                break;
            case 2:
                timeThisSection=random(longIntervalLowerBound,longIntervalUpperBound);
                break;
            default:
                break;
            }
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









