#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]){
    if(argc!=3){
        printf("Please provide paths to 2 files to compare");
    }

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    if(file1.fail()){
        printf("Could not find file: ",argv[1]);
    }

    if(file2.fail()){
        printf("Could not find file: ",argv[2]);
    }

    string line1, line2;
    int i = 0;

    while(getline(file1,line1)){
        if(!getline(file2,line2)){
            printf("LINE %d: LINES ARE DIFFERENT!\n",i);
        }
        if(line1 == line2){
            printf("LINE %d: OK\n",i);
        }
        else{
            printf("LINE %d: LINES ARE DIFFERENT!\n",i);
        }

        i++;
    }
    while(getline(file2,line2)){
        printf("LINE %d: LINES ARE DIFFERENT!\n",i);
        i++;
    }
    file1.close();
    file2.close();

}