#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <stdio.h>
int main(void){
	int a=0xA0003FE4;
	int b = a & 0x1FFFFFFF;
	printf("%x\n", b );
}