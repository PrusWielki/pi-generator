#include <stdio.h>

#include <cmath> // sqrt function

#include <gmpxx.h> // allows for the usage of gmp arbitrary long precision numbers, and for the standard notation of formulas (e.g. a=b*c*d instead of bc=b*c, a=bc*d)
 // #include <mpfr.h> //mpfr proved to have no efficiency improvements over standard gmp
// #include <mpf2mpfr.h>
#include <chrono> // for time measurement

using namespace std;

// struct used for the return value of recursive binary_splitting function
struct resultstruct {

  mpz_class P, Q, T;
};

// constants are defiend by the Chudnovsky algorithm: https://en.wikipedia.org/wiki/Chudnovsky_algorithm
namespace constants {

  const uint A = 640320,
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

int main(int argc, char ** argv) {
  if (argc <= 1) {
    printf("arg1=number_of_digits");
    return 1;
  }

  auto start = chrono::high_resolution_clock::now();

  int number_of_digits = atoi(argv[1]);

  // Chudnovsky algorithm produces a little bit over 14.18 digits per iteration: https://mathoverflow.net/questions/261162/chudnovsky-algorithm-and-pi-precision
  double digits_per_iteration = 14.18;

  // precision is the amount of bits that are going to be used to represent the obtained pi: https://math.stackexchange.com/questions/160295/how-many-bits-needed-to-store-a-number
  int precision = log2(10) *number_of_digits; 
  int iterations = number_of_digits / digits_per_iteration + 1;

  mpf_set_default_prec(precision);

  mpf_class pi;
  pi = 0;

  resultstruct result = binary_splitting(0, iterations);



  pi = constants::D * sqrt((mpf_class) constants::F) * result.Q;


  pi /= (constants::E * result.Q + result.T);


  FILE * f = fopen("pi.txt", "w");
  if (f == NULL) {
    printf("Error opening file!\n");
    return 1;
  }

  gmp_fprintf(f, "%.*Ff", number_of_digits, pi);

  auto finish = chrono::high_resolution_clock::now();
  chrono::duration < double > elapsed = finish - start;
  printf("time: %f\n", elapsed.count());

  return 0;
}