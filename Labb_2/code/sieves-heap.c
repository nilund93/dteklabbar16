/*
	sieves.c
	Assignment 3b, Lab 2, IS1500
	By Sofie Borck Janeheim & Niclas Lund
	Fall 2016
*/

//Allocate the data on the heap instead of the stack.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>	//needed for sqrt-function

#define COLUMNS 6
int columncheck=0;

void print_number(int n){
    if(columncheck % COLUMNS ==0 && columncheck != 0){
      printf("\n");
  	}
  columncheck++;
  printf("%10d ", n);
}

void print_sieves(int n){
	/*
		Pseudocode can be found at:
		https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes#Pseudocode
		*/
	int *prime;
	prime = malloc(n * sizeof(char));
	if(!prime){
		printf("%s\n", "Not enough memory" );
	}
	for(int i = 2; i < n; i++){ //start on 2.
		prime[i] = 1;	//Fill every position with ones
	}

	for (int i = 2; i < sqrt(n); i++){
		if(prime[i] == 1){
			for(int j = i*i; j < n; j = j + i){
				prime[j] = 0;
			}
		
		}
	}
	for(int i=0; i < n; i++){
		if(prime[i]==1){
      			print_number(i);
      	}
	}
	//free the memory
	free(prime);
}

int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}