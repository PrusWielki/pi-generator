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
  input.close();

  ofstream ofs;
  ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
  
  ofs <<digits;

  ofs.close();


}



int main(int argc, char ** argv) {
  if (argc <= 8) {
    printf("arg1=number_of_digits, arg2=range_lower_bound, arg3=range_upper_bound, arg4=file_name_to_save_pi, arg5=file_name_to_save_table, arg6=function_lower_bound, arg7=function_upper_bound, arg8=file_name_pi_generate_table");
    return 1;
  }

  auto start = chrono::high_resolution_clock::now();

  int number_of_digits = atoi(argv[1]);

  // Chudnovsky algorithm produces a little bit over 14.18 digits per iteration: https://mathoverflow.net/questions/261162/chudnovsky-algorithm-and-pi-precision
  double digits_per_iteration = 14.18;

  // precision is the amount of bits that are going to be used to represent the obtained pi: https://math.stackexchange.com/questions/160295/how-many-bits-needed-to-store-a-number
  long precision = log2(10) *number_of_digits; 
  int iterations = number_of_digits / digits_per_iteration + 3;

  mpf_set_default_prec(precision);

  mpf_class pi;
  pi = 0;

  resultstruct result = binary_splitting(0, iterations);



  pi = constants::D * sqrt((mpf_class) constants::F) * result.Q;


  pi /= (constants::E * result.Q + result.T);


  FILE * f = fopen(argv[4], "w");
  if (f == NULL) {
    printf("Error opening file!\n");
    return 1;
  }

  gmp_fprintf(f, "%.*Ff", number_of_digits, pi);
  fclose(f);

  DotRemoval(argv[4]);


  auto finish = chrono::high_resolution_clock::now();
  chrono::duration < double > elapsed = finish - start;
  printf("time: %f\n", elapsed.count());

  // -------------------------------
  // Generate table
  ifstream input;
  string pattern;
  input.open(argv[8]); 
  stringstream strStream;
  strStream << input.rdbuf(); 
  string digits = strStream.str(); 
  digits= digits.substr(atoi(argv[2]),atoi(argv[3])-atoi(argv[2]));
  string str;

  FILE * f2 = fopen(argv[5], "w");
  if (f2 == NULL) {
    printf("Error opening file!\n");
    return 1;
  }
  
  for(int i =atoi(argv[6]); i<atoi(argv[7]);i++) {
    str =to_string(i);
    int *prefSuf = (int*)calloc(str.length() + 2,sizeof(int));
    InitStrongPrefSuf(str,prefSuf);
    fprintf(f2, "%d,%d\n", i, KMP(digits,str,prefSuf));
    // printf("%d, %d\n", i ,KMP(digits,str,prefSuf));
  }
  fclose(f2);




  // ----------------------------------------------------------------
  // Pattern Searching

  while(true){
    printf("Pattern: ");

    cin>>pattern;
    //pattern = "14";
    //printf("Pattern: ");

    int *prefSuf2 = (int*)calloc(pattern.length() + 2,sizeof(int));
    InitStrongPrefSuf(pattern,prefSuf2);
    printf("%d\n",KMP(digits,pattern,prefSuf2));
  }

  
  return 0;
}
