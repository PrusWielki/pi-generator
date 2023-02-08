#include <stdio.h>

#include <cmath> // sqrt function

#include <gmpxx.h> // allows for the usage of gmp arbitrary long precision numbers, and for the standard notation of formulas (e.g. a=b*c*d instead of bc=b*c, a=bc*d)
 // #include <mpfr.h> //mpfr proved to have no efficiency improvements over standard gmp
// #include <mpf2mpfr.h>
#include <chrono> // for time measurement

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>

using namespace std;

// struct used for the return value of recursive binary_splitting function
struct resultstruct {

  mpz_class P, Q, T;
};

// constants are defiend by the Chudnovsky algorithm: https://en.wikipedia.org/wiki/Chudnovsky_algorithm
namespace constants {

  const int A = 640320,
    B = 13591409,
    C = 545140134,
    D = 426880,
    E = 13591409,
    F = 10005;

}
// C324 is used by binary_splitting function, in order to avoid passing it in each call it'd declared as global
mpz_class C = constants::A, C324 = C * C * C / 24;

// Chudnovsky binary splitting adapted from: https://www.craig-wood.com/nick/articles/pi-chudnovsky/
resultstruct binary_splitting(mpz_class a, mpz_class b) {

  mpz_class m;
  resultstruct result;

  if (1 == b - a) {
    if (0 == b) {
      result.P = 1;
      result.Q = 1;
    } else {
      result.P = (6 * b - 5) * (2 * b - 1) * (6 * b - 1);
      result.Q = b * b * b * C324;

    }

    result.T = result.P * (constants::B + constants::C * b);
    if ((b & 1) == 1) { // check if b is even
      result.T = -1 * result.T;
    }

  } else {

    m = (a + b) / 2;
    resultstruct result_a_m = binary_splitting(a, m);
    resultstruct result_m_b = binary_splitting(m, b);

    result.P = result_a_m.P * result_m_b.P;
    result.Q = result_a_m.Q * result_m_b.Q;

    result.T = result_a_m.T * result_m_b.Q + result_m_b.T * result_a_m.P;

  }
  return result;

}

// ----------------------------------------------------------------
// Pattern Searching
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

void DotRemoval(string filename){

  ifstream input;
  string pattern;
  input.open(filename); 
  stringstream strStream;
  strStream << input.rdbuf(); 
  string digits = strStream.str(); 
  digits.erase(1,1);
  digits.pop_back(); 
  input.close();

  ofstream ofs;
  ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
  
  ofs <<digits;

  ofs.close();


}



int main(int argc, char ** argv) {
  //if (argc <= 8) {
    //printf("arg1=number_of_digits, arg2=range_lower_bound, arg3=range_upper_bound, arg4=file_name_to_save_pi, arg5=file_name_to_save_table, arg6=function_lower_bound, arg7=function_upper_bound, arg8=file_name_pi_generate_table");
    //return 1;
  //}
while(1){
  cout<< "Choose from one of the options below:"<<endl<<"1. Generate PI"<<endl<<"2. Generate f_pi table"<<endl<<"3. Compare two files"<<endl<<"4. Quit"<<endl<<"Choice: ";
  int choice=0;
  cin>>choice;
switch(choice){
  case 1:
{



  auto start = chrono::high_resolution_clock::now();

  int number_of_digits = 0;//atoi(argv[1]);
  cout<<"Write the number of digits you want to generate: ";
  cin>>number_of_digits;
  cout<<"Enter file name to save pi to: ";
  string filename;
  cin>>filename;
  const int filename_length = filename.length();
  char* filename_array = new char[filename_length + 1];
  strcpy(filename_array, filename.c_str());
  // Chudnovsky algorithm produces a little bit over 14.18 digits per iteration: https://mathoverflow.net/questions/261162/chudnovsky-algorithm-and-pi-precision
  double digits_per_iteration = 14;

  // precision is the amount of bits that are going to be used to represent the obtained pi: https://math.stackexchange.com/questions/160295/how-many-bits-needed-to-store-a-number
  long precision = log2(10) *(number_of_digits+3); 
  long iterations = number_of_digits / digits_per_iteration + 2;

  mpf_set_default_prec(precision);

  mpf_class pi;
  pi = 0;

  resultstruct result = binary_splitting(0, iterations);



  pi = constants::D * sqrt((mpf_class) constants::F) * result.Q;


  pi /= (constants::E * result.Q + result.T);

  cout<<filename_array;
  FILE * f = fopen(filename_array, "w");
  if (f == NULL) {
    printf("Error opening file!\n");
    return 1;
  }

  gmp_fprintf(f, "%.*Ff", number_of_digits+1, pi);
  fclose(f);

  DotRemoval(filename_array);


  auto finish = chrono::high_resolution_clock::now();
  chrono::duration < double > elapsed = finish - start;
  printf("\ntime: %f\n", elapsed.count());




  break;
}
  case 2:
  {
  // -------------------------------
  // Generate table
  string tablefilename;
  string pifilename;
  cout<<"Provide file name of a file with pi: ";
  cin>>pifilename;
  cout<<"Provide file name to save table to: ";
  cin>>tablefilename;

  const int pifilename_length = pifilename.length();
  char* pifilename_array = new char[pifilename_length + 1];
  strcpy(pifilename_array, pifilename.c_str());
  const int tablefilename_length = tablefilename.length();
  char* tablefilename_array = new char[tablefilename_length + 1];
  strcpy(tablefilename_array, tablefilename.c_str());


  cout<<"Provide lowest argument, for example: 0: ";
  int lowerbound=0;
  cin>>lowerbound;
  cout<<"Provide lowest argument, for example: 100: ";
  int upperbound=0;
  cin>>upperbound;



  ifstream input;
  string pattern;
  input.open(pifilename_array); 
  stringstream strStream;
  strStream << input.rdbuf(); 
  string digits = strStream.str(); 
  // digits= digits.substr(atoi(argv[2]),atoi(argv[3])-atoi(argv[2]));
  string str;

  FILE * f2 = fopen(tablefilename_array, "w");
  if (f2 == NULL) {
    printf("Error opening file!\n");
    return 1;
  }
  
  for(int i =lowerbound; i<=upperbound;i++) {
    str =to_string(i);
    int *prefSuf = (int*)calloc(str.length() + 2,sizeof(int));
    InitStrongPrefSuf(str,prefSuf);
    fprintf(f2, "%d,%d\n", i, KMP(digits,str,prefSuf));
    // printf("%d, %d\n", i ,KMP(digits,str,prefSuf));
  }
  fclose(f2);


  break;
  }
  case 3:{
        cout<<"file name of the first file: ";
        string filename1;
        string filename2;
        cin>>filename1;
         cout<<"file name of the second file: ";
  cin>>filename2;
  string outputFileName;
  cout<<"file name to output results: ";
  cin>>outputFileName;

    ifstream file1(filename1);
    ifstream file2(filename2);

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

    ofstream outputFile(outputFileName);  

    string line1, line2;
    char c1,c2;
    int i = 0;
    outputFile<<"info: postion is indexed from 1"<<endl;

    while(file1.good()&&file2.good()) {
            file1.get(c1);
            file2.get(c2);
            if(c1!=c2){
                outputFile<<"files differ at element: "<<i+1<<endl;
                // cout<<"files differ at element: "<<i+1<<endl;
            }
            i++;
        }
    file1.close();
    file2.close();
    break;
}
  case 4:{

  exit(0);
  break;}
}
  

}

  






  
  return 0;
}
