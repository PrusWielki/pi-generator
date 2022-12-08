#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
using namespace std;

void InitStrongPrefSuf(string str, int prefSuf[]){
    prefSuf[0] = -1;
    int prefSufLength = -1;
    int stringLength = str.length();

    for(int i=1;i<=stringLength;i++){
        while(prefSufLength > -1 && str[prefSufLength] != str[i-1]){
            prefSufLength = prefSuf[prefSufLength];
        }
        prefSufLength+=1;
        if(str[i] != str[prefSufLength] || i == stringLength){
            prefSuf[i] = prefSufLength;
        }
        else{
            prefSuf[i] = prefSuf[prefSufLength];
        }
    }
}

int KMP(string digits, string pattern, int prefSuf[]){
    int prefSufLength = 0;
    int digitsLength = digits.length();
    int patternLength = pattern.length();

    for(int i=0; i<digitsLength; i++){
        while(prefSufLength > -1 && digits[i] != pattern[prefSufLength]){
            prefSufLength = prefSuf[prefSufLength];
        }
        prefSufLength +=1;
        if(prefSufLength == patternLength){
           return  i - prefSufLength +1;
        }
    }
    return -1;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Please pass 2 arguments");
        return 0;
    }

    ifstream input;
    input.open(argv[1]); 
    stringstream strStream;
    strStream << input.rdbuf(); 
    string digits = strStream.str(); 

    string pattern = argv[2];

    int *prefSuf = (int*)calloc(pattern.length() + 2,sizeof(int));
    InitStrongPrefSuf(pattern,prefSuf);
    printf("%d\n",KMP(digits,pattern,prefSuf));

}