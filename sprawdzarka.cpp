#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]){
    if(argc!=3){
        printf("Please provide paths to 2 files to compare");
        return 0;
    }

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    if(file1.fail()){
        file1.close();
        file2.close();
        cout<<"Could not find file: " << argv[1]<<endl;
        return 0;
    }

    if(file2.fail()){
        file1.close();
        file2.close();
        cout<<"Could not find file: " << argv[2]<<endl;
        return 0;
    }

    ofstream outputFile("cmpResults.txt");  

    string line1, line2;
    char c1,c2;
    int i = 0;

    while(file1.good()&&file2.good()) {
            file1.get(c1);
            file2.get(c2);
            if(c1!=c2){
                outputFile<<"files differ at element: "<<i;
                // cout<<"files differ at element: "<<i;
            }
            i++;
        }
    file1.close();
    file2.close();

}