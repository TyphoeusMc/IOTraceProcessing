//本程序模拟多个监控摄像头的IO情况
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define tracesNumber 20   //序列的数量
#define traceLength 2500   //假定总序列长度为1天
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

int longEvent = 0;    //每次发生事件持续的时间是否长 0：短 1：混合 2：长
int longInterval = 0;    //没两次时间之间的间隔是否长  0：短 1：混合 2：长
bool variate = false;    //各次事件的发生的时间长度差别是否大（相差时间30%）
int timeCounter = 0;    //总时间段之内的秒数计数
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
    int totalSeconds;     //事件发生的总秒数
    srand((unsigned)time(NULL));
    for(int trNumber=0; trNumber<tracesNumber; trNumber++)
    {
        totalSeconds = 0;
        lastTime = 0;
        timeLeft = traceLength;
        timeThisSection = 0;
        firstEventLength = -1;
        //确定本序列属性
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
            //先随机一个事件长度
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
            //此处输出的是本次事件的结束时间
            result<<actualTime<<",";
            timeLeft=(timeLeft>=timeThisSection)?timeLeft-timeThisSection:0;
            if(timeLeft<=0)
                break;
            //后随机一个空闲时间
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
            //此处输出的是下一次事件的开始时间
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














