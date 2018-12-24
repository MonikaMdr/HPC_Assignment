#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <crypt.h>
#include "time_diff.h"

int n_passwords = 4;
pthread_t t1, t2;
char *encrypted_passwords[] = {

"$6$KB$LmvMGhIR4PnYKlPXAOo2K4zt.BEjUN/9fKPMYT1zyisqo4WxY.FKfiCGz15GAoyNV5dW3GP.uQSiIaBQ3zxgo0",

"$6$KB$twSU8eqeVwdeUxOdMWkrWYzOuqqTW7r.Urc6uQocmeDZk0MYBwj14lTCSYyX8N3mcMrzWX/SqeKRQPLuOXABC1",

"$6$KB$6aOGghFdbd/KlpjgarEiXZkABucvI1Pklj7qygVFJXDGoVU4oTR.HYGJ4eYoucH6atToVXxeahhjjKfXwAz7n0",

"$6$KB$.ur/R5Tth0qt7kcs68A.eOyui5DzRH4W9nUydGsq27SQ0YlAf95KIzoDNxYcJYTxkj6y76WJn/eoY3ytucjfi/"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
  
void *kernel_function1(void *salt_and_encrypted){

int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(x='A'; x<='M'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		printf("Thread1");
		sprintf(plain, "%c%c%02d",x, y, z);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		  printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	    }
	  }
  printf("%d solutions explored\n", count);

}
void *kernel_function2(void *salt_and_encrypted){

int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(x='N'; x<='Z'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		printf("Thread2");
		sprintf(plain, "%c%c%02d", x, y, z);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		  printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	    }
	  }
  printf("%d solutions explored\n", count);

}

int main(int argc, char *argv[]){
  int i;
struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i=0;i<n_passwords;i<i++) {
   pthread_create(&t1, NULL, kernel_function1, encrypted_passwords[i]);
  pthread_create(&t2, NULL, kernel_function2, encrypted_passwords[i]);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);		

  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);
  printf("run lasted %9.5lfs\n", difference/1000000000.0);
  return 0;
}
