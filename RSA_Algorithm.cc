// RSA Assignment for ECE4122/6122 Fall 2015
// Name: Amit Kulkarni GTID: 903038158

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

using namespace std;

mpz_class p,q;
// Implement the RSA_Algorithm methods here

// Constructor
RSA_Algorithm::RSA_Algorithm()
  : rng(gmp_randinit_default)
{
  // get a random seed for the random number generator
  int dr = open("/dev/random", O_RDONLY);
  if (dr < 0)
    {
      cout << "Can't open /dev/random, exiting" << endl;
      exit(0);
    }
  unsigned long drValue;
  read(dr, (char*)&drValue, sizeof(drValue));
  //cout << "drValue " << drValue << endl;
  rng.seed(drValue);
// No need to init n, d, or e.
}

// void GenerateRandomKeyPair(size_t sz);

void RSA_Algorithm::GenerateRandomKeyPair(size_t sz)
{
  mpz_class phi, e_, d_, val;
  mpz_class p1,q1;

  size_t sz_ = 2*sz;
  //cout<<"sz_ "<<sz_<<endl;
  computePrime(p,sz);
  computePrime(q,sz);

  //cout<<"P: "<<p<<endl;
  //cout<<"Q: "<<q<<endl;

  // Compute n = p*q;
  //mpz_mul(n, p, q);
  n = p*q;

  // p1 = (p-1);
  //mpz_sub_ui(p1, p, 1);
  p1 = (p-1);

  // q1 = (q-1);
  //mpz_sub_ui(q1, q, 1);
  q1 = (q-1);

  // Compute phi = p1 * q1;
  //mpz_mul(phi, p1, q1);
  phi = p1*q1;

  //cout<<"phi "<<phi<<endl;
  // Computation of public key 'e'
  bool flag = false;
  while(flag != true)
  {
    //cout<<"Inside while loop"<<endl;
    computePrime(e_,sz_); // Select random value of e
    mpz_gcd(val.get_mpz_t(), e_.get_mpz_t(), phi.get_mpz_t()); // gcd(e,phi) == 1
    if((val == 1) && (e_ < phi))
      flag = true;
    else
      flag = false;
  }
  e = e_; // Public key exponent

  // Computation of private key 'd'
  // d is multiplicative inverse of e mod(phi).
  mpz_invert(d_.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
  d = d_;
}

mpz_class RSA_Algorithm::Encrypt(mpz_class M)
{
  // Here, 'e' is public key.
  mpz_class cipher;

  // generating cipher or encrypted message 'encryptM'
  mpz_powm(cipher.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  return cipher; //  return cipher.get_mpz_t()?  -----> check
}

mpz_class RSA_Algorithm::Decrypt(mpz_class C)
{
  // Here, 'd' is private key.
  mpz_class decryptM;

  // generating cipher or encrypted message 'encryptM'
  mpz_powm(decryptM.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  return decryptM; //  return decryptM.get_mpz_t()?  -----> check
}

mpz_class RSA_Algorithm::RhoFactor(mpz_class N)
{
    /*  Algorithm

      g(x) = (x2 + 1) mod n
      x ← 2; y ← 2; d ← 1; // Step 1: Initialize
      While d = 1:
            x ← g(x)
            y ← g(g(y))
            d ← gcd(|x - y|, n)   // Step 2: Calculate GCD
      If d = n, return failure.
      Else, return d.
    */

    // Step 1: Initialize
    mpz_class x = 2;
    mpz_class y = 2;
    mpz_class d = 1;

    while(d == 1)
    {
      mpz_class g_y; // ---> check
      mpz_class abs_val;
      x = ((x * x) + 1) % N;
      g_y = ((y * y) + 1) % N;
      y = ((g_y * g_y) + 1) % N;

      if(y > x)
        abs_val = y - x;
      else
        abs_val = x - y;

      // get gcd // Step 2: get GCD
      mpz_gcd( d.get_mpz_t() , abs_val.get_mpz_t() , N.get_mpz_t());

    }

    return d;
}


// Fill in the remainder of the RSA_Algorithm methods
// void computePrime(mpz_class& x, size_t);
// --> function to select two distinct prime numbers

void RSA_Algorithm::computePrime(mpz_class& x, size_t sz)
{
  bool flag = false;

  while(flag!=true)
  {
    x = rng.get_z_bits(sz);
    if(mpz_probab_prime_p(x.get_mpz_t(),100) == 0) // 100 iterations
      flag = false;
    else
      flag = true;
  }
}

void RSA_Algorithm::PrintND()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(mpz_class M)
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(mpz_class C)
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}
