//������ģ�����������ͷ��IO���
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define tracesNumber 20   //���е�����
#define traceLength 2500   //�ٶ������г���Ϊ1��
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

int main()
{
    ofstream result;
    result.open("C:\\Users\\MartinPC\\Desktop\\TraceCreated\\event.txt");
    ofstream tracePlot;
    tracePlot.open("C:\\Users\\MartinPC\\Desktop\\TraceCreated\\plot.txt");
    tracePlot<<"header"<<endl;
    int timeLeft;
    int timeThisSection;
    int firstEventLength;
    int actualTime;
    int totalSeconds;     //�¼�������������
    srand((unsigned)time(NULL));
    for(int trNumber=0; trNumber<tracesNumber; trNumber++)
    {
        totalSeconds = 0;
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
        result<<0<<",";
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
            totalSeconds+=(timeLeft>=timeThisSection)?timeThisSection+1:timeLeft+1;
            for(int k=lastTime; k<=actualTime; k++)
                tracePlot<<k<<",";
            lastTime=actualTime;
            //cout<<timeThisSection<<endl;
            //�˴�������Ǳ����¼��Ľ���ʱ��
            result<<actualTime<<",";
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
            result<<actualTime<<",";
            timeLeft=(timeLeft>=timeThisSection)?timeLeft-timeThisSection:0;
        }
        tracePlot<<endl;
        for(int k=0; k<totalSeconds; k++)
            tracePlot<<trNumber+1<<",";
        result<<endl;
        tracePlot<<endl;
    }
    result.close();
    return 0;
}














