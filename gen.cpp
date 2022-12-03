#include<stdio.h>
#include <stdlib.h>
#include <gmp.h>
using namespace std;

int main(int argc, char** argv)
{
    if(argc<=3) {
        printf("arg1=precision, arg2=number_of_iterations arg3=amount_of_numbers");
        return 1;
     }

    int precision = atoi(argv[1]);
    int number_of_iterations = atoi(argv[2]);
    int amount_of_numbers= atoi(argv[3]);

    mpf_set_default_prec(precision);

    mpf_t k, a_k, a_sum, b_sum, C, C3,C324, total, pi;

    mpf_inits(k, a_k, a_sum, b_sum,C, C3, C324, total, pi, NULL);

    mpf_set_ui(k,1);
    mpf_set_ui(a_k,1);
    mpf_set_ui(a_sum,1);
    mpf_set_ui(C,640320);

    mpf_pow_ui(C3,C,3);
    mpf_div_ui(C324,C3,24);




    mpf_t eq1, eq1_1, eq2, eq2_1, eq3, eq3_1;

    mpf_inits(eq1, eq1_1, eq2, eq2_1, eq3, eq3_1, NULL);


    for(int i=0; i<number_of_iterations;i++)
        // while(true)
        {
        // a_k *= -(6*k-5)*(2*k-1)*(6*k-1)
        // eq1 = -(6*k-5)
        // eq1_1 = 6*k
        // etc

        // eq1
        mpf_mul_ui(eq1_1,k,6);
        mpf_sub_ui(eq1,eq1_1,5);
        mpf_neg(eq1,eq1);
        // eq2
        mpf_mul_ui(eq2_1,k,2);
        mpf_sub_ui(eq2,eq2_1,1);
        // eq3
        mpf_mul_ui(eq3_1,k,6);
        mpf_sub_ui(eq3,eq3_1,1);
        // a_k
        mpf_mul(a_k,a_k,eq1);
        mpf_mul(a_k,a_k,eq2);
        mpf_mul(a_k,a_k,eq3);


        // a_k /= k*k*k*C324
        mpf_div(a_k,a_k,k);
        mpf_div(a_k,a_k,k);
        mpf_div(a_k,a_k,k);
        mpf_div(a_k,a_k,C324);


        // a_sum += a_k
        mpf_add(a_sum,a_sum,a_k);

        // b_sum += k*a_k
        // eq1= k * a_k
        mpf_mul(eq1, k, a_k);
        mpf_add(b_sum,b_sum, eq1);

        // k+=1
        mpf_add_ui(k,k,1);

        if(0 == mpf_cmp_ui(a_k,0))
            break;

    }
    // total = 13591409*a_sum + 545140134*b_sum
    // eq1 = 13591409*a_sum
    // eq2 = 545140134*b_sum
    mpf_mul_ui(eq1, a_sum, 13591409);
    mpf_mul_ui(eq2, b_sum, 545140134);
    mpf_add(total, eq1,eq2);

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
        mpf_clears(k, a_k, a_sum, b_sum,C, C3, C324, total, pi, eq1, eq1_1, eq2, eq2_1, eq3, eq3_1, NULL);
        return 1;
    }

    gmp_fprintf(f,"%.*Ff", amount_of_numbers, pi);


    mpf_clears(k, a_k, a_sum, b_sum,C, C3, C324, total, pi, eq1, eq1_1, eq2, eq2_1, eq3, eq3_1, NULL);
    return 0;
}
