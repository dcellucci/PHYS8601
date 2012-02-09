#include "stdio.h"
#include "r1279.cc"
#include "stdbool.h"
//Dr. Landau's 8601 class, homework problem 1
//Program by Daniel Cellucci, 2012

main(){

  int stagesize = 10, stage[stagesize][stagesize], groupkey[(int)(stagesize*stagesize/2)];
  int xrand, yrand, temp, seed, groupnum, assign;
  FILE *out1;
  seed = 6750235;
	out1  = fopen("out_put.out", "w");
  rinitialize(seed);
  groupnum = 1;
  assign = 0;

  for(int i = 0; i < stagesize; i++){
    for(int j = 0; j < stagesize; j++){
      stage[i][j]=0;     
    }
  }
  
  //populating the board with items
  for(int i = 0; i < (int)(stagesize*stagesize/2); i++){
    groupkey[i] = 0;
    xrand = (int)(ranf()*10.0);
    yrand = (int)(ranf()*10.0);
    if(stage[xrand][yrand]==1)
      i--;
    else
      stage[xrand][yrand]=1;
  }

  for(int i = 0; i < stagesize; i++){
    for(int j = 0; j < stagesize; j++){
      if(stage[i][j] != 0){
        int grpval = groupnum;
        int bckleft = 0, bckrgt = 0;
        if(i-1 >= 0)
          bckleft = stage[i-1][j];
        if(j-1 >= 0)
          bckrgt = stage[i][j-1];
        if(bckleft == 0 && bckrgt == 0){
          stage[i][j] = groupnum;
          groupkey[groupnum+1] = groupnum+1;
          groupnum++;
        }
        else if(bckleft == 0 || bckrgt == 0)
          stage[i][j] = bckleft+bckrgt;
        else if(bckleft == bckrgt)
          stage[i][j] = bckleft;
        else if(bckleft < bckrgt && bckleft != 0){
          stage[i][j] = bckleft;
          bool looking = true;
          int tempdex = bckrgt;
          while(looking){
            if(groupkey[tempdex] > 0 && looking){
              looking = false;
              groupkey[tempdex] = -bckleft;
            }
            else
              tempdex = bckleft;
          }
        }
        else if(bckrgt < bckleft && bckrgt != 0){
          stage[i][j] = bckrgt;
          bool looking = true;
          int tempdex = bckleft;
          while(looking){
            if(groupkey[tempdex] > 0 && looking){
              looking = false;
              for(int i = 0; i < (int)(stagesize*stagesize/2); i++)
                if(groupkey[i] != 0)
                  printf("%i %i \n", i, groupkey[i]);
              groupkey[tempdex] = -bckrgt;
            }
            else if (groupkey[tempdex] == 0)
              looking = false;
            else
              tempdex = -groupkey[tempdex];
          }
        }
      }
    }
  }

  for(int i = 0; i < stagesize; i++){
    for(int j = 0; j < stagesize; j++){
      fprintf(out1, "%02d  ", stage[i][j]);     
    }
    fprintf(out1, "\n");
  }
  for(int i = 0; i < (int)(stagesize*stagesize/2); i++)
    if(groupkey[i] != 0)
      fprintf(out1, "%i %i \n", i, groupkey[i]);
}


