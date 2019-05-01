#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#define threshold 500      //选择记录了150条trace以上的进程
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

int main()
{
    //创建一个txt文件用于合并
    ofstream trace;
    trace.open("C:\\Users\\MartinPC\\Desktop\\data2\\trace.txt");
    //逐个读取txt文件，将所有文件合并并将其中的时间处理成以秒为单位的时间
    ifstream inTXT;
    int sequence=0;
    unsigned int sTimePos=0, eTimePos=0;
    int timePos=0;
    int totalLines=0;
    string lineRead;
    string initTime;
    traceInfo infoRead;
    double minutes=0, seconds=0, totalTime=0;
    //存储着所有trace，左值为trace本身，右值为pid
    vector<pair<string,traceInfo>> timeProcessedLines;
    //存储着每个pid对应的trace数量
    map<string,int> pidPool;
    string pid;
    stringstream s2n;
    //最终结果
    map<string,vector<int>> finalResult;
    double upperBound=1;

    //设置精度
    s2n.precision(10);
    for(string path:allPath)
    {
        cout<<"processing "<<sequence<<endl;
        inTXT.open(path.c_str());
        if(!inTXT.is_open())
            cout<<"fail to open: "<<path<<endl;
        while(getline(inTXT,lineRead))
        {
            //截取时间
            sTimePos=lineRead.find_first_of(',',20);
            sTimePos++;
            eTimePos=lineRead.find_first_of(',',sTimePos);
            timePos=lineRead.find_first_of(':',sTimePos);
            initTime=lineRead.substr(timePos+1,eTimePos-timePos-1);
            //处理其中的分钟和秒钟
            s2n<<initTime.substr(0,2);
            s2n>>minutes;
            s2n.clear();
            timePos=initTime.find_first_of(':',0);
            s2n<<initTime.substr(timePos+1,eTimePos-timePos-1).c_str();
            s2n>>seconds;
            s2n.clear();
            //去除异常数据
            if(seconds>=60)
                continue;
            totalTime=sequence*60*60+minutes*60+seconds;
            s2n<<totalTime;
            s2n>>initTime;
            s2n.clear();
            infoRead.time=totalTime;
            unsigned int sPos,ePos;
            //取出ioSize
            sPos=lineRead.find_first_of(',',eTimePos+1);
            sPos=lineRead.find_first_of(',',sPos+1);
            ePos=lineRead.find_first_of(',',sPos+1);
            s2n<<lineRead.substr(sPos+1,ePos-sPos-1);
            s2n>>infoRead.ioSize;
            s2n.clear();
            //取出pid
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
    vector<int> test;
    test.clear();
    for(pair<string,int> a:pidPool)
    {
        if(a.second>=threshold)
        {
            finalResult[a.first]=test;
            cout<<a.first<<" "<<a.second<<endl;
            pSelected++;
        }
    }
    //筛选数据，将trace数量小于一定值的进程的记录筛去,并提取目标特征
    /*
    1. ave read size
    2. ave write size
    3. read/write ratio
    4. total r/w times
    5. r/w density
    6. io bandwith per second (size density)
    7. io times per second
    */
    //扫描trace
    for(pair<string,traceInfo> a:timeProcessedLines)
    {
        //不考虑trace数量小于阈值的process
        if(pidPool[a.second.pid]<threshold)
            continue;
        lineSelected++;
        //comb<<a.first<<endl;
        //comb_extract<<"trace info: "<<a.second.time<<" "<<a.second.ioSize<<" "<<a.second.isRead<<" "<<a.second.pid<<endl;
        //若当前trace的时间超过上界，窗口向前移动一秒，并综合先前数据
        if(a.second.time>=upperBound)
        {
            while(a.second.time>=upperBound)
                upperBound++;
        }
        if(finalResult[a.second.pid].empty()||(upperBound>*(finalResult[a.second.pid].end()-1)))
            finalResult[a.second.pid].push_back(upperBound);
    }
    //计算最大长度
    int maxLength=0;
    for (auto a:finalResult)
        if(a.second.size()>maxLength)
            maxLength=a.second.size();
    for(int k=0; k<maxLength; k++)
        trace<<k<<",";
    trace<<endl;
    int number=0, time=1;
    int left2Output=0;
    for(auto a:finalResult)
    {
        for(int b:a.second)
        {
            trace<<b<<",";
            number++;
        }
        left2Output=maxLength-number;
        for(int k=0; k<left2Output; k++)
            trace<<0<<",";
        trace<<endl;
        for(int k=0; k<number; k++)
            trace<<time<<",";
        for(int k=0; k<left2Output; k++)
            trace<<0<<",";
        trace<<endl;
        time++;
        number=0;
    }
    cout<<"press any key to close"<<endl;
    getchar();
    return 0;
}
