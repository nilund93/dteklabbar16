/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <math.h>	

int is_prime(int n){
	//Skriv om denna kod så att det returneras en etta ifall det är ett primtal
	//och en nolla om det inte är det.
	//Använd endast enkla loopar och if-satser
	int flag=1;
	for(int i=2; i <= sqrt(n); i++){
		//Om delbart så är det ej ett primtal
		if(n % i == 0){

			flag = 0;
			break;
		}
		else{
			flag = 1;
		}
	}
	//Om flaggad, alltså ett primtal, returnera 1.
	if(flag == 1 && n != 0 && n != 1){
		return 1;
	}
	else{
		return 0;
	}
}

int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
  printf("%d\n", is_prime(1));
  printf("%d\n", is_prime(0));
  printf("%d\n", is_prime(2));
}
