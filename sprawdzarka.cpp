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
    int i = 0;

    while(getline(file1,line1)){
        outputFile<<i<<" ";
        if(!getline(file2,line2)){
            outputFile << "LINES ARE DIFFERENT!" <<endl;
        }
        if(line1 == line2){
            outputFile << "OK"<<endl;
        }
        else{
            outputFile << "LINES ARE DIFFERENT!" <<endl;
        }

        i++;
    }
    while(getline(file2,line2)){
        outputFile << i << " LINES ARE DIFFERENT!" <<endl;
        i++;
    }
    file1.close();
    file2.close();

}