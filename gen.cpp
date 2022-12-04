#include<stdio.h>
#include <stdlib.h>
#include <cmath>
#include <gmpxx.h>
// #include <mpfr.h>
// #include <mpf2mpfr.h>
#include <chrono> 

using namespace std;

struct resultstruct {

    mpf_class P, Q ,T;
};



resultstruct binary_splitting(int a, int b, mpf_class C324){

     int m;
     resultstruct result;

     if(1==b-a){
        if(0==a){
        result.P=1;
        result.Q=1;}
    else{
        result.P= (6*a-5)*(2*a-1)*(6*a-1);
        result.Q=a*a*a*C324;
    }
    result.T=result.P*(13591409 + 545140134*a);
    if(a&1){ // check if a is even
        result.T=-1*result.T;
    }

     }
     else{

        m=(a+b)/2;
        resultstruct result_a_m=binary_splitting(a,m,C324);
        resultstruct result_m_b=binary_splitting(m,b,C324);

        result.P=result_a_m.P+result_m_b.P;
        result.Q=result_a_m.Q+result_m_b.Q;
        result.T=result_a_m.T*result_m_b.Q+result_m_b.T*result_a_m.Q;;



     }
     return result;
    
    
}


int main(int argc, char** argv)
{
    if(argc<=1) {
        printf("arg1=number_of_digits");
        return 1;
     }

    auto start = chrono::high_resolution_clock::now();

    int number_of_digits = atoi(argv[1]);

    double digits_per_iteration = log(53360^3) / log(10);

    int precision =  log2(10) *number_of_digits; //log2(15)*(number_of_digits-2)+1;
    int iterations = number_of_digits/digits_per_iteration +1;

    mpf_set_default_prec(precision);

    mpf_class C,C324, sqrtC;
    mpf_class pi;

    pi=0;
    C=640320;
    C324=C*C*C/24;
    sqrtC=sqrt(C);



     resultstruct result = binary_splitting(0,iterations,C324);

    
     pi = (result.Q*(mpf_class)426880*sqrtC)/result.T;


     gmp_fprintf(stdout,"%d", result.T);

    FILE *f = fopen("file.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }

      gmp_fprintf(f,"%.*Ff", number_of_digits, pi);
     // mpfr_out_str (f, 10, 0, pi, MPFR_RNDD);



    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    printf("time: %f\n", elapsed.count());
    
    return 0;
}
