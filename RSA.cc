// ECE4122/6122 RSA Encryption/Decryption assignment
// Fall Semester 2015
// Name: Amit Kulkarni GTID: 903038158

#include <iostream>
#include "RSA_Algorithm.h"

using namespace std;

int main()
{
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // Step 1: Loop from sz = 32 to 1024 inclusive
  // Step 2: for each size choose 10 different key pairs
  // Step 3: For each key pair choose 10 differnt plaintext
  // Step 4: messages making sure it is smaller than n.
  // Step 5: If not smaller then n then choose another
  // Step 6: For eacm message encrypt it using the public key (n,e).
  // step 7: After encryption, decrypt the ciphertext using the private
  // step 8: key (n,d) and verify it matches the original message.

  // your code here

  //Step 1
  for(size_t sz = 32; sz <= 1024; sz = 2*sz)
  {
    //Step 2
    for(int i = 0; i < 10; i++) // Loop for 10 public/private keys
    {
        RSA.GenerateRandomKeyPair(sz);
        RSA.PrintNDE();   // Printing n, d and e for each key pair

        // Step 3
        for(int m = 0; m < 10; m++) // loop to generate random messages
        {
          //cout<<"Message iteration: "<<m<<endl;

          bool flag = false;
          mpz_class message,c,recvM;

          while(flag == false)
          {
            message = RSA.rng.get_z_bits(2*sz);
            //Step 4
            if(message < RSA.n)
              flag = true;
            else
              flag = false; // Step 5
          }
          //cout<<"message received"<<endl;
          RSA.PrintM(message);  // ---------> Print each message
          //cout<<"Encrypting..."<<endl;
          //RSA.PrintNE();
          //Step 6: Encryption
          c = RSA.Encrypt(message);
          RSA.PrintC(c);      // -----------> Print cipher text
          //cout<<"Decrypting..."<<endl;
          //RSA.PrintND();
          //Step 7: Decryption
          recvM = RSA.Decrypt(c);

          //Step 8: verification
          if(recvM == message)
          {
      //      cout<<"Success for message iteration: "<<m<<endl;
          }
          else
            cout<<"Encryption failed"<<endl;
        }

    //    cout<<"Success for iteration: "<<i<<endl;
    }

  //  cout<<"Success for size: "<<sz<<endl;
  }
  return 0;
}
