#include "stdio.h"
#include "stdlib.h"
#include "r1279.cc"
#include "stdbool.h"
//Dr. Landau's 8601 class, homework problem 1
//Program by Daniel Cellucci, 2012

//initialization of variables. First the stage, then the temp
//and bookkeeping values.
#define stagesize 10
static int stage[stagesize][stagesize];
static int groupkey[stagesize];

int find_parent(int child);
bool is_infinite();
void run_a_job();

int find_parent(int child){
  if(groupkey[child] > 0)
    return child;
  else if(groupkey[child] < 0)
    find_parent(-1*(int)groupkey[child]);
  else
    return 0;
}

bool is_infinite(){
  unsigned int border[4][stagesize];
  int flag = 0;
  bool inf = false;
  for(int j = 0; j < 4; j++){
    for(int i = 0; i < stagesize; i++){
      if(j == 0)
        border[j][i] = find_parent(stage[i][0]);
      if(j == 1)
        border[j][i] = find_parent(stage[stagesize-1][i]);
      if(j == 2)
        border[j][i] = find_parent(stage[i][stagesize-1]);
      else
        border[j][i] = find_parent(stage[0][i]);
    }
  }
  for(int k = 0; k < stagesize; k++){
    if(border[0][k] != 0){
      flag = 0;
      for(int j = 1; j < 4; j++){
        int i = 0;
        bool not_found = true;
        while(not_found && i < stagesize){
          if(border[j][i] == border[0][k]){
            not_found = false;
            flag++;
          }
          else
            i++;
        }
      }
      if(flag == 3)
        inf = true;
    }
  }
  return inf;
}

void run_a_job(){
  int xrand, yrand, temp, groupnum, assign, bckleft, bcktop;
  float ratio = 0.59;
  //note groupnum is 1. 
  groupnum = 1;
  assign = 0;
  //emptying the stage
  for(int i = 0; i < stagesize; i++){
    for(int j = 0; j < stagesize; j++){
      stage[i][j]=0;     
    }
  }
  //populating the board with items note that I attempt to fill
  //half the board with non-zero values, regardless of accidental
  //degeneracies.
  for(int i = 0; i < (int)(stagesize*stagesize*ratio); i++){
    groupkey[i] = 0;
    xrand = (int)(ranf()*stagesize);
    yrand = (int)(ranf()*stagesize);
    if(stage[xrand][yrand]==1)
      i--;
    else
      stage[xrand][yrand]=1;
  }
  //Now to check for clusters
  for(int i = 0; i < stagesize; i++){
    for(int j = 0; j < stagesize; j++){
      //only runs if there is something at (i,j)
      if(stage[i][j] != 0){
        //initial setup of temp variables,
        bckleft = 0;
        bcktop = 0;
        //checking if there are neighbors to assign 
        if(i > 0)
          bckleft = stage[i-1][j];
        if(j > 0)
          bcktop = stage[i][j-1];
        //i.e. if it is in the upper left corner or it is isolated 
        //in the top and left directions
        if(bckleft == 0 && bcktop == 0){
          //replacing the 1 with a group label
          stage[i][j] = groupnum;
          //indicating that this item is isolated in the group key
          groupkey[groupnum] = groupnum;
          //need a unique key for the next isolated item.
          groupnum++;
        }
        //since we know one of our neighbors is filled, we need to
        //check if one is empty. Since this checks if one is empty,
        //a simple sum will give us the nonzero group key value.
        else if(bckleft == 0 || bcktop == 0)
          stage[i][j] = bckleft+bcktop;
        //now to check if both neighbors are in the same cluster.
        else if(bckleft == bcktop)
          stage[i][j] = bckleft;
        //now here is the complicated part. What if there are two 
        //non-zero, inequal neighbors? First checking if the left is
        //less than the upper.
        else if(bckleft < bcktop){
          //we want to assign the lower of the two
          stage[i][j] = bckleft;
          bool looking = true;
          int tempdex = bcktop;
          //we also want to relabel the groupkey so that the larger
          //neighbor is reassigned to the smaller group as well.
          while(looking){
            //we want to keep going through the reference chain until
            //we reach a positive group assignment value.
            if(groupkey[tempdex] > 0){
              looking = false;
              if(bckleft < tempdex)
                groupkey[tempdex] = -bckleft;
              //we then assign the group key at this final value to be
              //the negative of the smaller, thus extending the call
              //chain by one link
            }
            //unless we hit 0 0 (we shouldn't), which means we have to
            //stop because we will hit an infinite loop.
            else if (groupkey[tempdex] == 0)
              looking = false;
            //if all else fails, that means the index reference is 
            //negative, and therefore we need to travel up the ref
            //tree in order to find the source
            else
              tempdex = -1*(int)groupkey[tempdex];
          }
        }
        else if(bcktop < bckleft){
          stage[i][j] = bcktop;
          bool looking = true;
          int tempdex = bckleft;
          while(looking){
            if(groupkey[tempdex] > 0){
              looking = false;
              if(bcktop < tempdex)
                groupkey[tempdex] = -bcktop;
            }
            else if (groupkey[tempdex] == 0)
              looking = false;
            else
              tempdex = -1*(int)groupkey[tempdex];
          }
        }
      }
    }
  }
}

void main(){
  
  //Programming chaismus. I could have reordered it, but it's
  //the little things in life. 
  FILE *out1;
  int seed = 6750236;
  out1  = fopen("out_put.out", "w");
  rinitialize(seed);
  float temp = 0;
  int count = 0;
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 10; j++){
      printf("i");
      run_a_job();
      if(is_infinite()){
        count++;
      }
    }
    temp+=count/10.0;
  }
  fprintf(out1,"%6.4f",temp/100.0);
}










/*
      for(int i = 0; i < stagesize; i++){
        for(int j = 0; j < stagesize; j++){
          if(stage[i][j] > 0)
            fprintf(out1, "%02d  ", stage[i][j]); 
          else
            fprintf(out1, "**  ", stage[i][j]);    
        }
        fprintf(out1, "\n");
      }
      for(int i = 0; i < (int)(stagesize*stagesize/2); i++)
        if(groupkey[i] != 0)
          fprintf(out1, "%i %i %i \n", i, groupkey[i], find_parent(i));
          */


