#include <iostream>
#include <fstream>
#include <string>
#include <map>
#define threshold 2000//选择记录了100条trace以上的进程

using namespace std;

int main()
{
    ifstream txtIn;
    txtIn.open("C:\\Users\\MartinPC\\Desktop\\data2\\comb.txt");
    if(!txtIn.is_open())
        cout<<"failed to open"<<endl;
    string lineRead;
    int position,position2;
    string pid;
    map<string,int> pidPool;
    while(getline(txtIn,lineRead))
    {
        position=lineRead.find_first_of('R',50);
        if(position==string::npos)
            position=lineRead.find_first_of('W',50);
        position2=lineRead.find_first_of(',',position+2);
        pid=lineRead.substr(position+2,position2-position-2);
        if(pidPool.find(pid)==pidPool.end())
            pidPool[pid]=1;
        else
            pidPool[pid]++;
    }
    int moreT=0;
    for(pair<string,int> a:pidPool)
        if(a.second>threshold)
        {
            cout<<a.first<<" "<<a.second<<endl;
            moreT++;
        }
    cout<<"size:"<<pidPool.size()<<endl;
    cout<<moreT<<endl;
    return 0;
}


