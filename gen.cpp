#include<stdio.h>
#include <stdlib.h>
#include <cmath>
#include <gmpxx.h>
// #include <mpfr.h>
// #include <mpf2mpfr.h>
#include <chrono> 

using namespace std;

int main(int argc, char** argv)
{
    if(argc<=1) {
        printf("arg1=number_of_digits");
        return 1;
     }

    int number_of_digits = atoi(argv[1]);
    int precision =  log2(15)*(number_of_digits-2)+1;
    int iterations = number_of_digits/14.18 +1;

    mpf_set_default_prec(precision);

    mpf_class k, a_k, a_sum, b_sum, C, C3,C324, total, pi;

   b_sum=0;
   total=0;
   pi=0;
   k=1;
   a_k=1;
   a_sum=1;
   C=640320;
   C3=C*C*C;
   C324=C3/24;
    

    auto start = chrono::high_resolution_clock::now();

     for(int i=0; i<iterations;i++)
     // while(true)
        {
        // a_k *= -(6*k-5)*(2*k-1)*(6*k-1)
        a_k *= -(6*k-5)*(2*k-1)*(6*k-1);
        
        // a_k /= k*k*k*C324
        a_k = a_k/( k*k*k*C324);

        // a_sum += a_k
        a_sum += a_k;

        // b_sum += k*a_k
        b_sum += k*a_k;

        // k+=1
        k+=1;


        // check if we achieved the desired accuracy
        //  mpf_mul_ui(eq1,a_k,number_of_digits*number_of_digits);
        //  gmp_printf ("%.*Ff\n", number_of_digits, eq1);
        //  mpf_floor(eq1,eq1);
        //  gmp_printf ("%.*Ff\n", number_of_digits, eq1);
        //  if(0 == mpf_cmp_ui(eq1,0))
        //     break;

    }
    // total = 13591409*a_sum + 545140134*b_sum
    total = 13591409*a_sum + 545140134*b_sum;

    // pi = (426880*sqrt(10005*one, one)*one) / total
    pi = (426880*sqrt((mpf_class)10005)) / total;

    // gmp_printf ("%.*Ff", 10000, pi);



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
