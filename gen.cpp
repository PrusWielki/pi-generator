#include<stdio.h>
#include <stdlib.h>
#include <cmath>
#include <gmp.h>
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

    // int one = 1;

    // int precision = atoi(argv[1]);
    int number_of_digits = atoi(argv[1]);
    // int amount_of_numbers= atoi(argv[3]);

    int precision =  log2(15)*(number_of_digits-2)+1;

    int iterations = number_of_digits/14.18 +1;

    // printf("%d\n", precision);


    mpf_set_default_prec(precision);

    mpf_t k, a_k, a_sum, b_sum, C, C3,C324, total, pi;

    mpf_inits(k, a_k, a_sum, b_sum,C, C3, C324, total, pi, NULL);

    mpf_set_ui(b_sum,0);
    mpf_set_ui(total,0);
    mpf_set_ui(pi,0);

    mpf_set_ui(k,1);
    mpf_set_ui(a_k,1);
    mpf_set_ui(a_sum,1);
    mpf_set_ui(C,640320);

    mpf_pow_ui(C3,C,3);
    mpf_div_ui(C324,C3,24);
    // mpf_floor(C324,C324);




    mpf_t eq1, eq1_1, eq2, eq2_1, eq3, eq3_1;

    mpf_inits(eq1, eq1_1, NULL);
    

    auto start = chrono::high_resolution_clock::now();

     for(int i=0; i<iterations;i++)
     // while(true)
        {
        // a_k *= -(6*k-5)*(2*k-1)*(6*k-1)
        // eq1 = -(6*k-5)
        // eq1_1 = 6*k
        // etc

        
        mpf_mul_ui(eq1_1,k,6);
        mpf_sub_ui(eq1,eq1_1,5);
        mpf_neg(eq1,eq1);


        mpf_mul(a_k,a_k,eq1);
        
        mpf_mul_ui(eq1_1,k,2);
        mpf_sub_ui(eq1,eq1_1,1);

        mpf_mul(a_k,a_k,eq1);
        
        mpf_mul_ui(eq1_1,k,6);
        mpf_sub_ui(eq1,eq1_1,1);
        
        mpf_mul(a_k,a_k,eq1);



        // a_k /= k*k*k*C324
        mpf_div(a_k,a_k,k);
        mpf_div(a_k,a_k,k);
        mpf_div(a_k,a_k,k);
        mpf_div(a_k,a_k,C324);
        // mpf_floor(a_k,a_k);

        // a_sum += a_k
        mpf_add(a_sum,a_sum,a_k);

        // b_sum += k*a_k
        // eq1= k * a_k
        mpf_mul(eq1, k, a_k);
        mpf_add(b_sum,b_sum, eq1);

        // k+=1
        mpf_add_ui(k,k,1);


        // check if we achieved the desired accuracy
        //  mpf_mul_ui(eq1,a_k,number_of_digits*number_of_digits);
        //  gmp_printf ("%.*Ff\n", number_of_digits, eq1);
        //  mpf_floor(eq1,eq1);
        //  gmp_printf ("%.*Ff\n", number_of_digits, eq1);
        //  if(0 == mpf_cmp_ui(eq1,0))
        //     break;

    }
    // total = 13591409*a_sum + 545140134*b_sum
    // eq1 = 13591409*a_sum
    // eq2 = 545140134*b_sum
    mpf_mul_ui(eq1, a_sum, 13591409);
    mpf_mul_ui(eq1_1, b_sum, 545140134);
    mpf_add(total, eq1,eq1_1);


    // pi = (426880*sqrt(10005*one, one)*one) / total
    // eq1 = 426880*sqrt(10005)
    // eq1_1 = sqrt(10005)
    mpf_sqrt_ui(eq1_1,10005);
    mpf_mul_ui(eq1,eq1_1, 426880);
    mpf_div(pi,eq1,total);


    // gmp_printf ("%.*Ff", 10000, pi);



    FILE *f = fopen("file.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        mpf_clears(k, a_k, a_sum, b_sum,C, C3, C324, total, pi, eq1, eq1_1, NULL);
        return 1;
    }

     gmp_fprintf(f,"%.*Ff", number_of_digits, pi);
     // mpfr_out_str (f, 10, 0, pi, MPFR_RNDD);

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    printf("time: %f\n", elapsed.count());

    mpf_clears(k, a_k, a_sum, b_sum,C, C3, C324, total, pi, eq1, eq1_1, NULL);
    return 0;
}
