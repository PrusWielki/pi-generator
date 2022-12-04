#include <stdio.h>
#include <cmath> // sqrt function
#include <gmpxx.h> // allows for the usage of gmp arbitrary long precision numbers, and for the standard notation of formulas (e.g. a=b*c*d instead of bc=b*c, a=bc*d)
// #include <mpfr.h> //mpfr proved to have no efficiency improvements over standard gmp
// #include <mpf2mpfr.h>
#include <chrono> // for time measurement

using namespace std;


// struct used for the return value of recursive binary_splitting function
struct resultstruct {

    mpz_class P, Q ,T;
};

namespace constants{

const uint A = 640320,B = 13591409,C = 545140134,D = 426880,E = 13591409, F = 10005;

}

mpz_class C = constants::A,C324 = C*C*C/24;


resultstruct binary_splitting(mpz_class a, mpz_class b){

     mpz_class m;
     resultstruct result;

     if(1==b-a){
        if(0==b){
            result.P =1;
            result.Q=1;
        }
    else{
        result.P= (6*b-5)*(2*b-1)*(6*b-1);
        result.Q=b*b*b*C324;
        
        if(result.Q==0){
        gmp_printf ("Qa: %Zd\n",result.Q);
printf("b: %d\n",b);
gmp_printf ("C324: %Zd\n",C324);
        }
        }

        
    
    result.T=result.P*(constants::B + constants::C*b);
    if((b&1)==1){ // check if a is even
        result.T=-1*result.T;
    }

     }
     else{

        m=(a+b)/2;
        resultstruct result_a_m=binary_splitting(a,m);
        resultstruct result_m_b=binary_splitting(m,b);

        result.P=result_a_m.P*result_m_b.P;
        result.Q=result_a_m.Q*result_m_b.Q;
        
        result.T=result_a_m.T*result_m_b.Q+result_m_b.T*result_a_m.P;



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

    double digits_per_iteration = 14.12; // log(53360^3) / log(10);

    int precision =  log2(15)*(number_of_digits-2)+1; // log2(10) *number_of_digits; //
    int iterations = number_of_digits/digits_per_iteration +1;

    mpf_set_default_prec(precision);

    
    mpf_class pi;

    pi=0;
    



     resultstruct result = binary_splitting(0,iterations);

    
     pi = constants::D * sqrt((mpf_class)constants::F)*result.Q;
     
    // gmp_fprintf(stdout,"%d\n", pi);
     pi /=(constants::E*result.Q+result.T);


     // gmp_printf ("%Zd\n",result.T);
     // gmp_printf ("Q: %Zd\n",result.Q);


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
