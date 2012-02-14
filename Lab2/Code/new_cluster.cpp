#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdio.h>
#include "mt19937-64.c"
#include "math.h"

using namespace std;

typedef std::vector<double> dbl_list;
typedef std::vector<int> int_list;
typedef std::map<int,int> histogram;

std::vector<int_list> stage;
int_list groupkey;
histogram cluster_dist;

int find_parent(int child){
  if(groupkey[child] > 0)
    return child;
  else if(groupkey[child] < 0)
    find_parent(-groupkey[child]);
  else
    return 0;
}

void gen_dist(){
  for(int i = 0; i < groupkey.size(); i++){
    if(groupkey[i]>0)
      cluster_dist[groupkey[i]]++;
  }

}

bool is_infinite(){
  int_list lft,rgt,top,bot;
  int length = stage.size();
  bool inf = false;
  lft.resize(groupkey.size(),0);
  rgt.resize(groupkey.size(),0);
  top.resize(groupkey.size(),0);
  bot.resize(groupkey.size(),0);
  
  for(int i = 0; i < length; i++)
    lft[find_parent(stage[i][0])] = 1;
    
  for(int i = 0; i < length; i++)
    rgt[find_parent(stage[length-1][i])] = 1;
    
  for(int i = 0; i < length; i++)
    top[find_parent(stage[i][length-1])] = 1;
    
  for(int i = 0; i < length; i++)
    bot[find_parent(stage[0][i])] = 1;
  
  for(int i = 1; i < lft.size(); i++){
    if(lft[i]==1 && rgt[i]==1 && top[i]==1 && bot[i]==1)
      inf = true;
    //if(groupkey[i]!=0)
      //printf("%02d  %02d  %02d  %02d  %02d  %02d\n",i,groupkey[i],lft[i],rgt[i],top[i],bot[i]);
  }
  return inf;
}
void check_for_clusters(){
  int bcklft,bcktop;
  int groupnum = 1;
  int minlab = 0;
  int maxlab = 0;
  /*
  for(int i = 0; i < stage.size(); i++){
    for(int j = 0; j < stage[i].size(); j++){
      if(stage[i][j]==0)
        printf("**  ");
      else
        printf("01  ");
    }
    cout << "\n";
  }
  cout << "\n\n";
  //*/
  for(int i = 0; i < stage.size(); i++){
    for(int j = 0; j < stage[i].size(); j++){
      bcklft = 0;
      bcktop = 0;
      if(stage[i][j]!=0){
        if(i > 0)
          bcklft = find_parent(stage[i-1][j]);
        if(j > 0)
          bcktop = find_parent(stage[i][j-1]);
        if(bcklft == 0 && bcktop == 0){
          stage[i][j] = groupnum;
          groupkey[groupnum]++;
          groupnum++;
        }
        else if(bcklft == 0 || bcktop == 0){
          stage[i][j] = bcklft+bcktop;
          groupkey[bcklft+bcktop]++;
        }
        else if(bcklft == bcktop){
          stage[i][j] = bcklft;
          groupkey[bcklft]++;
        }
        else{
          minlab = (bcklft < bcktop) ? bcklft : bcktop;
          maxlab = (bcklft > bcktop) ? bcklft : bcktop;
           
          stage[i][j] = minlab;
          groupkey[minlab] = groupkey[bcktop]+groupkey[bcklft]+1;
          groupkey[maxlab] = -minlab;
        }
      }
    }
  }/*
  for(int i = 0; i < stage.size(); i++){
    for(int j = 0; j < stage[i].size(); j++){
      if(stage[i][j]==0)
        printf("**  ");
      else
        printf("%02d  ",stage[i][j]);
    }
    cout << "\n";
  }
  cout << "\n\n";
  //for(int i = 0; i < groupkey.size(); i++){
    //if(groupkey[i]!=0)
      //printf("%02d  %02d\n",i,groupkey[i]);
  //}//*/
}

int main(){
  //initialization of Mersenne Twister
  //Random Number Generator
  unsigned long long seed2 = 28347124120192834;
  init_genrand64(seed2);
  
  //initialization of simulation parameters
  int stagesize = 100;
  int maxruns = 1000;
  int samples = 10;
  double ratio = 0.59;
  
  stage.resize(stagesize);
  groupkey.resize(stagesize*stagesize/2+1);
  
  //Temporary count of infinite clusters
  int count = 0;
  double sum = 0;
  double sumsquares = 0;
  double err = 0;
  
  //initialization of File I/O
  ofstream myfile;
  stringstream stst;
  stst << stagesize;
  string s = stst.str().append(".pts");
  myfile.open(s.c_str());

  //for(double ratio = 0.0; ratio < 1.0; ratio+=0.01){
    
    err = 0;
    sum = 0; 
    sumsquares = 0;
    for(int m = 0; m < samples; m++){ 
      count = 0;
      for(int num = 0; num < maxruns; num++){
        std::fill(groupkey.begin(),groupkey.end(),0);
        for(int i = 0; i < stage.size(); i++){
          stage[i].resize(stagesize);
          for(int j = 0; j < stage[i].size(); j++){
            if(genrand64_real2()<ratio)
              stage[i][j] = 1;
            else        
              stage[i][j] = 0; 
          }
        }
        check_for_clusters();
        gen_dist();
      }
      //sum += 1.0*count/maxruns;
      //sumsquares += 1.0*(count*count)/(maxruns*maxruns);
    }
    //float mean = 1.0*sum/samples;
    //err = sqrt((sumsquares-sum*mean)/(samples*samples));
    //cout  << ratio <<","<< mean << " " << err << "\n";
    //myfile<< ratio <<","<< mean << ",\n";
  //}
  map<int,int>::iterator it;
  it = cluster_dist.begin()++;
  long double normalize = 1.0/it->second;
  for(it = cluster_dist.begin(); it!=cluster_dist.end(); it++)
    myfile << it->first << " " << (it->second)*1.0*normalize << "\n"; 
  myfile.close();
  return 0;
}
