#include "stdio.h"
#include "r1279.cc"
//Dr. Landau's 8601 class, homework problem 1
//Program by Daniel Cellucci, 2012

main(){
//initialization of the variables
  FILE *out1,*rtest;
  int count, maxcall, maxiter, seed;
	double xrand, yrand, dist, pi, avg;
	
//opening for file I/O
	out1  = fopen("out_pi.out", "w");
	rtest = fopen("rtest.out","w");

//assignment of the initial values, including the
//initial seed value.
  count = 0;
  avg = 0;
  maxcall = 1000000;
  maxiter = 100;
  seed = 660522;

//two loops, one for calculating individual values
//of pi by generating random coordinates in a box,
//the other for resetting the seed value to keep 
//things fresh. 
  for(int j = 0; j < maxiter; j++){
    count = 0;
    rinitialize(seed);
	  for(int i = 0; i < maxcall; i++){
      xrand = ranf()-0.5;     //getting a random 
                              //coordinate and 
                              //centering it
      yrand = ranf()-0.5;     //ditto but with y
      if(j == 0 && i < 1000)
        fprintf(rtest,"%9.7f %9.7f \n",xrand,yrand);
      dist = xrand*xrand+yrand*yrand;
      if(dist < 0.25)         //if the value is 
        count++;              //w/i the circle's
    }                         //area, increment
    pi = 4.0*count/maxcall;
    avg = avg + pi;
    printf("%i %9.7f \n", j, pi); 
    fprintf(out1, "%i %9.7f \n", j, pi);     
    seed = ranf()*10000000;   //reset the seed
  }
  avg = avg/maxiter;
  printf("\n %11.9f \n", avg);//output final val
  fprintf(out1,"\n %9.7f \n", avg); 
	return 0;
}
