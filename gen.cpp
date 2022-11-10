#include <iostream>
#include <algorithm>
#include <ctime>
#include <sys/time.h>
#include<random>

using namespace std;

int R(int a, int b) {return a+rand()%(b-a+1);}

int main(int argc, char *argv[])
{
    timeval czas;
    gettimeofday(&czas, 0);
    srand(czas.tv_sec*1000+czas.tv_usec%1000);
    int a = 1;
    int b = 45;
    cout<<R(a,b)<<endl;
}
