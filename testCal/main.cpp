#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

//此程序用于测试均值与均方差的递推计算算法的准确性

int main()
{
    double ave=0, std_v=0, ave_r=0, std_v_r=0;
    double ave_old;
    vector<int> input;
    double inGet=0;
    int inSeq=1;
    for(int i=0; i<5; i++)
    {
        cin>>inGet;
        input.push_back(inGet);
        ave_old=ave_r;
        ave_r=ave_r+((inGet-ave_r)/inSeq);
        std_v_r=std_v_r+(inGet-ave_old)*(inGet-ave_r);
        inSeq++;
    }
    double sum_std=0;
    for(int a:input)
        ave+=a;
    ave/=5;
    for(int a:input)
        std_v+=(a-ave)*(a-ave);
    std_v/=4;
    std_v=sqrt(std_v);
    std_v_r/=4;
    std_v_r=sqrt(std_v_r);
    cout<<ave<<" "<<ave_r<<" "<<std_v<<" "<<std_v_r<<endl;
    getwchar();
    return 0;
}
