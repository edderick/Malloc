#include <stdio.h>
#include "mymalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);

	printf("%d", result);
}
