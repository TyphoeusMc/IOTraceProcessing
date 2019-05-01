#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#define threshold 150      //ѡ���¼��150��trace���ϵĽ���
#define searchStartPos 50

/*
�޸ģ�
1.��pid����ʱ��ʼ�ſ�ʼͳ�ƣ�����ȫ��
2.��׼��ֻͳ����io����������
3.����������ioռ�ձȡ�
*/

using namespace std;

typedef struct traceInfo
{
    double time;
    double ioSize;
    bool isRead;
    string pid;
}traceInfo;

typedef struct processStats
{
    double rTime=0;
    double wTime=0;
    double ioSecond=0;
    double totalSeconds=0;
    long double rSizePSMax=0;
    long double wSizePSMax=0;
    long double ioSizePSMax=0;
    double rTimePSMax=0;
    double wTimePSMax=0;
    double ioTimePSMax=0;
    double aveRSize=0;
    double aveWSize=0;
    double aveIOSize=0;
    double aveRTime=0;
    double aveWTime=0;
    double aveIOTime=0;
    double stdVR=0;
    double stdVW=0;
    double stdVIO=0;
    double stdVR_s=0;
    double stdVW_s=0;
    double stdVIO_s=0;
}processStats;

typedef struct processStatsPS
{
    double rTime=0;
    double wTime=0;
    double rSize=0;
    double wSize=0;
}processStatsPS;

double upperBound=1;

ofstream test;

vector<string> allPath={
"C:\\Users\\MartinPC\\Desktop\\data2\\text11400.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11401.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11402.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11403.txt"
/*
"C:\\Users\\MartinPC\\Desktop\\data2\\text11404.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11405.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11406.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11407.txt",
//"C:\\Users\\MartinPC\\Desktop\\data2\\text11408.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11409.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11410.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11411.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11412.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11413.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11414.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11415.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11416.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11417.txt",
//"C:\\Users\\MartinPC\\Desktop\\data2\\text11418.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11419.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11420.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11421.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11422.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11423.txt"
*/
};

void mergeResult(map<string,processStats> &finalResult, map<string,processStatsPS> &resultPerSecond);

int main()
{
    //����һ��txt�ļ����ںϲ�
    ofstream comb;
    ofstream comb_extract;
    ofstream result;
    test.open("C:\\Users\\MartinPC\\Desktop\\data2\\shit.txt");
    comb.open("C:\\Users\\MartinPC\\Desktop\\data2\\comb_test2.txt");
    comb_extract.open("C:\\Users\\MartinPC\\Desktop\\data2\\comb_test2_e.txt");
    result.open("C:\\Users\\MartinPC\\Desktop\\data2\\result.txt");
    //�����ȡtxt�ļ����������ļ��ϲ��������е�ʱ�䴦�������Ϊ��λ��ʱ��
    ifstream inTXT;
    int sequence=0;
    unsigned int sTimePos=0, eTimePos=0;
    int timePos=0;
    int totalLines=0;
    string lineRead;
    string initTime;
    traceInfo infoRead;
    double minutes=0, seconds=0, totalTime=0;
    //�洢������trace����ֵΪtrace������ֵΪpid
    vector<pair<string,traceInfo>> timeProcessedLines;
    //�洢��ÿ��pid��Ӧ��trace����
    map<string,int> pidPool;
    string pid;
    stringstream s2n;
    //���ս��
    map<string,processStats> finalResult;
    map<string,processStatsPS> resultPerSecond;

    //���þ���
    s2n.precision(10);
    for(string path:allPath)
    {
        cout<<"processing "<<sequence<<endl;
        inTXT.open(path.c_str());
        if(!inTXT.is_open())
            cout<<"fail to open: "<<path<<endl;
        while(getline(inTXT,lineRead))
        {
            //��ȡʱ��
            sTimePos=lineRead.find_first_of(',',20);
            sTimePos++;
            eTimePos=lineRead.find_first_of(',',sTimePos);
            timePos=lineRead.find_first_of(':',sTimePos);
            initTime=lineRead.substr(timePos+1,eTimePos-timePos-1);
            //�������еķ��Ӻ�����
            s2n<<initTime.substr(0,2);
            s2n>>minutes;
            s2n.clear();
            timePos=initTime.find_first_of(':',0);
            s2n<<initTime.substr(timePos+1,eTimePos-timePos-1).c_str();
            s2n>>seconds;
            s2n.clear();
            //ȥ���쳣����
            if(seconds>=60)
                continue;
            totalTime=sequence*60*60+minutes*60+seconds;
            s2n<<totalTime;
            s2n>>initTime;
            s2n.clear();
            infoRead.time=totalTime;
            unsigned int sPos,ePos;
            //ȡ��ioSize
            sPos=lineRead.find_first_of(',',eTimePos+1);
            sPos=lineRead.find_first_of(',',sPos+1);
            ePos=lineRead.find_first_of(',',sPos+1);
            s2n<<lineRead.substr(sPos+1,ePos-sPos-1);
            s2n>>infoRead.ioSize;
            s2n.clear();
            //ȡ��pid
            for(sPos=searchStartPos;sPos<lineRead.length();sPos++)
                if(lineRead[sPos]=='R'||lineRead[sPos]=='W'||lineRead[sPos]=='r'||lineRead[sPos]=='w')
                    break;
            if(lineRead[sPos]=='R'||lineRead[sPos]=='r')
                infoRead.isRead=true;
            else
                infoRead.isRead=false;
            ePos=lineRead.find_first_of(',',sPos+2);
            pid=lineRead.substr(sPos+2,ePos-sPos-2);
            if(pidPool.find(pid)==pidPool.end())
                pidPool[pid]=1;
            else
                pidPool[pid]++;
            infoRead.pid=pid;
            lineRead.replace(sTimePos,eTimePos-sTimePos,initTime);
            timeProcessedLines.push_back(make_pair(lineRead,infoRead));
            totalLines++;
        }
        inTXT.close();
        cout<<"done"<<endl;
        cout<<totalLines<<" lines processed"<<endl;
        sequence++;
    }
    //
    int lineSelected=0, pSelected=0;
    cout<<"selected pid listed bellow:"<<endl;
    //���pid
    processStats stat;
    processStatsPS statPS;
    for(pair<string,int> a:pidPool)
    {
        if(a.second>=threshold)
        {
            //finalResult[a.first]=stat;
            cout<<a.first<<" "<<a.second<<endl;
            pSelected++;
        }
    }
    //ɸѡ���ݣ���trace����С��һ��ֵ�Ľ��̵ļ�¼ɸȥ,����ȡĿ������
    //ɨ��trace
    for(pair<string,traceInfo> a:timeProcessedLines)
    {
        //������trace����С����ֵ��process
        if(pidPool[a.second.pid]<threshold)
            continue;

        lineSelected++;
        //comb<<a.first<<endl;
        //comb_extract<<"trace info: "<<a.second.time<<" "<<a.second.ioSize<<" "<<a.second.isRead<<" "<<a.second.pid<<endl;
        //����ǰtrace��ʱ�䳬���Ͻ磬������ǰ�ƶ�һ�룬���ۺ���ǰ����
        if(a.second.time>=upperBound)
        {
            while(a.second.time>=upperBound)
                upperBound++;
            mergeResult(finalResult,resultPerSecond);
        }
        //���Ƚ�û�м����
        if(finalResult.find(a.second.pid)==finalResult.end())
            finalResult[a.second.pid]=stat;
        if(resultPerSecond.find(a.second.pid)==resultPerSecond.end())
            resultPerSecond[a.second.pid]=statPS;
        if(a.second.isRead)
        {
            resultPerSecond[a.second.pid].rTime++;
            resultPerSecond[a.second.pid].rSize+=a.second.ioSize;
        }
        else
        {
            resultPerSecond[a.second.pid].wTime++;
            resultPerSecond[a.second.pid].wSize+=a.second.ioSize;
        }
    }
    mergeResult(finalResult,resultPerSecond);
    cout<<"analyse done"<<endl;
    cout<<"total lines: "<<timeProcessedLines.size()<<endl;
    cout<<"line selected: "<<lineSelected<<endl;
    cout<<"total process: "<<pidPool.size()<<endl;
    cout<<"process selected: "<<pSelected<<endl;
    pidPool.clear();
    timeProcessedLines.clear();
    comb.close();
    comb_extract.close();
    cout<<"writing txt file"<<endl;
    //print headers
    result<<"pid  ave_r_size  ave_w_size  r/w ratio  io_times  max_io/s  max_io_size/s  ave_io_times/s"<<endl;
    result.precision(10);
    for(auto &p:finalResult)
    {
        //result<<p.second.aveRSize<<",";
        //result<<p.second.aveWSize<<",";
        result<<((p.second.aveWSize==0)?100:(double(p.second.aveRSize)/p.second.aveWSize))<<",";
        result<<p.second.aveIOSize<<",";
        //����ֵ��ô����
        result<<((p.second.wTime==0)?100:(double(p.second.rTime)/p.second.wTime))<<",";
        result<<p.second.aveIOTime<<",";
        result<<p.second.rSizePSMax<<",";
        result<<p.second.wSizePSMax<<",";
        result<<p.second.ioSizePSMax<<",";
        result<<p.second.rTimePSMax<<",";
        result<<p.second.wTimePSMax<<",";
        result<<p.second.ioTimePSMax<<",";
        //���������׼��
        if(p.second.ioSecond==1)
            p.second.stdVIO=p.second.stdVIO_s=p.second.stdVR=p.second.stdVR_s=p.second.stdVW=p.second.stdVW_s=0;
        else
        {
            p.second.stdVIO=sqrt(p.second.stdVIO/(p.second.ioSecond-1));
            p.second.stdVIO_s=sqrt(p.second.stdVIO_s/(p.second.ioSecond-1));
            p.second.stdVR=sqrt(p.second.stdVR/(p.second.ioSecond-1));
            p.second.stdVR_s=sqrt(p.second.stdVR_s/(p.second.ioSecond-1));
            p.second.stdVW=sqrt(p.second.stdVW/(p.second.ioSecond-1));
            p.second.stdVW_s=sqrt(p.second.stdVW_s/(p.second.ioSecond-1));
        }
        result<<p.second.stdVR<<",";
        result<<p.second.stdVW<<",";
        result<<p.second.stdVIO<<",";
        result<<p.second.stdVR_s<<",";
        result<<p.second.stdVW_s<<",";
        result<<p.second.stdVIO_s<<",";
        result<<p.second.ioSecond/p.second.totalSeconds<<endl;
    }
    result.close();
    cout<<"press any key to close"<<endl;
    getchar();
    return 0;
}

void mergeResult(map<string,processStats> &finalResult, map<string,processStatsPS> &resultPerSecond)
{
    double old_ave_r_s, old_ave_w_s, old_ave_io_s;
    double old_ave_r_t, old_ave_w_t, old_ave_io_t;
    double rTime=0;
    double wTime=0;
    double rSize=0;
    double wSize=0;
    for(auto &i:finalResult)
    {
        rTime=wTime=rSize=wSize=0;
        //������µ�ǰ���ڸý�����������
        if(resultPerSecond.find(i.first)!=resultPerSecond.end())
        {
            rTime=resultPerSecond[i.first].rTime;
            wTime=resultPerSecond[i.first].wTime;
            rSize=resultPerSecond[i.first].rSize;
            wSize=resultPerSecond[i.first].wSize;
            old_ave_r_s=i.second.aveRSize;
            old_ave_w_s=i.second.aveWSize;
            old_ave_io_s=i.second.aveIOSize;
            old_ave_r_t=i.second.aveRTime;
            old_ave_w_t=i.second.aveWTime;
            old_ave_io_t=i.second.aveIOTime;
            i.second.rTime+=rTime;
            i.second.wTime+=wTime;
            i.second.ioSecond++;
            test<<i.first<<","<<rSize+wSize<<endl;
            //ave_r=ave_r+((inGet-ave_r)/inSeq)
            //1.ƽ������С
            i.second.aveRSize+=(rSize-i.second.aveRSize)/i.second.ioSecond;
            //2.ƽ��д��С
            i.second.aveWSize+=(wSize-i.second.aveWSize)/i.second.ioSecond;
            //3.ƽ��IO��С
            i.second.aveIOSize+=(rSize+wSize-i.second.aveIOSize)/i.second.ioSecond;
            //5.ƽ��IO�������룩
            i.second.aveRTime+=(rTime-i.second.aveRTime)/i.second.ioSecond;
            i.second.aveWTime+=(wTime-i.second.aveWTime)/i.second.ioSecond;
            i.second.aveIOTime+=(rTime+wTime-i.second.aveIOTime)/i.second.ioSecond;
            //6.������С
            if(rSize>i.second.rSizePSMax)
                i.second.rSizePSMax=rSize;
            //7.���д��С
            if(wSize>i.second.wSizePSMax)
                i.second.wSizePSMax=wSize;
            //8.���IO��С
            if((rSize+wSize)>i.second.ioSizePSMax)
                i.second.ioSizePSMax=rSize+wSize;
            //9.��������
            if(rTime>i.second.rTimePSMax)
                i.second.rTimePSMax=rTime;
            //10.���д����
            if(wTime>i.second.wTimePSMax)
                i.second.wTimePSMax=wTime;
            //11.���IO����
            if((rTime+wTime)>i.second.ioTimePSMax)
                i.second.ioTimePSMax=rTime+wTime;
            //std_v_r=std_v_r+(inGet-ave_old)*(inGet-ave_r);
            //12.ÿ��������ı�׼��
            i.second.stdVR+=(rTime-old_ave_r_t)*(rTime-i.second.aveRTime);
            //13.ÿ��д�����ı�׼��
            i.second.stdVW+=(wTime-old_ave_w_t)*(wTime-i.second.aveWTime);
            //14.ÿ��IO�����ı�׼��
            i.second.stdVIO+=(rTime+wTime-old_ave_io_t)*(rTime+wTime-i.second.aveIOTime);
            //15.ÿ�ζ���С�ı�׼��
            i.second.stdVR_s+=(rSize-old_ave_r_s)*(rSize-i.second.aveRSize);
            //16.ÿ��д��С�ı�׼��
            i.second.stdVW_s+=(wSize-old_ave_w_s)*(wSize-i.second.aveWSize);
            //17.ÿ��IO��С�ı�׼��
            i.second.stdVIO_s+=(rSize+wSize-old_ave_io_s)*(rSize+wSize-i.second.aveIOSize);
        }
        i.second.totalSeconds++;
    }
    resultPerSecond.clear();
}
