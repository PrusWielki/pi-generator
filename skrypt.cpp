#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    system("g++ -o main main.cpp");
    system("g++ -o main2 main2.cpp");
    system("g++ -o gen gen.cpp");
    
    int i = 0;

    while(true)
    {
        i++;
        cout << i << endl;

        system("./gen > test.in");
        system("./main < test.in > test.wzor");
        system("./main2 < test.in > test.brut");
        int result = system("diff -bq test.wzor test.brut");
        if(result != 0)
        {
            cerr << "ERROR!!!" << endl;
            break;
        }
    }
  
}