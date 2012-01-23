#include "stdio.h"
#include "r1279.cc"

main(){
	double temp1,temp2;
	
	rinitialize(660522);
	
	temp1 = ranf();
	temp2 = ranf();

	printf("%9.7f  %9.7f \n", temp1, temp2);
	
	return 0;
}
