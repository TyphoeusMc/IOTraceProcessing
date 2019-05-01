#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#define threshold 150      //ѡ���¼��150��trace���ϵĽ���
#define searchStartPos 50

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
    int rTime=0;
    int wTime=0;
    long double rSize=0;
    long double wSize=0;
    long double maxSizePerSec=0;
    double maxTimePerSec=0;
}processStats;

vector<string> allPath={
"C:\\Users\\MartinPC\\Desktop\\data2\\text11400.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11401.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11402.txt",
"C:\\Users\\MartinPC\\Desktop\\data2\\text11403.txt",
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
};

void mergeResult(map<string,processStats> &finalResult, map<string,processStats> &resultPerSecond);

int main()
{
    //����һ��txt�ļ����ںϲ�
    ofstream comb;
    ofstream comb_extract;
    ofstream result;
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
    map<string,processStats> resultPerSecond;
    double upperBound=1;

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
    //��ʼ��������Ч��pid��Ϣ�������pid
    processStats stat;
    for(pair<string,int> a:pidPool)
    {
        if(a.second>=threshold)
        {
            finalResult[a.first]=stat;
            cout<<a.first<<" "<<a.second<<endl;
            pSelected++;
        }
    }
    //ɸѡ���ݣ���trace����С��һ��ֵ�Ľ��̵ļ�¼ɸȥ,����ȡĿ������
    /*
    1. ave read size
    2. ave write size
    3. read/write ratio
    4. total r/w times
    5. r/w density
    6. io bandwith per second (size density)
    7. io times per second
    */
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
        if(resultPerSecond.find(a.second.pid)==resultPerSecond.end())
            resultPerSecond[a.second.pid]=stat;
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
    for(pair<string,processStats> p:finalResult)
    {
        result<<p.first<<",";
        //�����ĸΪ������
        result<<((p.second.rTime==0)?0:(p.second.rSize/p.second.rTime))<<",";
        result<<((p.second.wTime==0)?0:(p.second.wSize/p.second.wTime))<<",";
        //����ֵ��ô������
        result<<((p.second.wTime==0)?100:(double(p.second.rTime)/p.second.wTime))<<",";
        result<<p.second.rTime+p.second.wTime<<",";
        result<<p.second.maxTimePerSec<<","<<p.second.maxSizePerSec<<",";
        result<<double(p.second.rTime+p.second.wTime)/(22*60*60)<<endl;
    }
    result.close();
    cout<<"press any key to close"<<endl;
    getchar();
    return 0;
}

void mergeResult(map<string,processStats> &finalResult, map<string,processStats> &resultPerSecond)
{
    for(pair<string,processStats> i:resultPerSecond)
    {
        finalResult[i.first].rSize+=i.second.rSize;
        finalResult[i.first].wSize+=i.second.wSize;
        finalResult[i.first].rTime+=i.second.rTime;
        finalResult[i.first].wTime+=i.second.wTime;
        if((i.second.rSize+i.second.wSize)>finalResult[i.first].maxSizePerSec)
            finalResult[i.first].maxSizePerSec=i.second.rSize+i.second.wSize;
        if((i.second.rTime+i.second.wTime)>finalResult[i.first].maxTimePerSec)
            finalResult[i.first].maxTimePerSec=i.second.rTime+i.second.wTime;
    }
    resultPerSecond.clear();
}
