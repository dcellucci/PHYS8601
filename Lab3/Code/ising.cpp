#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "mt19937-64.c"

using namespace std;

//type definitions to save space later on
typedef std::vector<double> dbl_list;
typedef std::vector<int> int_list;

dbl_list isingEs;


std::vector<int_list> lattice;
ofstream myfile;

void mc_step(){
	//for(int i = 0; i < 100*lattice.size()*lattice.size(); i++){
	for(int i = 0; i < lattice.size(); i++){
		for(int j = 0; j < lattice.size(); j++){
			int tmpx = i;//(int)(genrand64_real2()*lattice.size());
			int tmpy = j;//(int)(genrand64_real2()*lattice.size());
			int sum = 0;

			if(tmpx > 0)
				sum = sum + lattice[tmpx][tmpy]*lattice[tmpx-1][tmpy];
			else
				sum = sum + lattice[tmpx][tmpy]*lattice[lattice.size()-1][tmpy];

			if(tmpy > 0)
				sum = sum + lattice[tmpx][tmpy]*lattice[tmpx][tmpy-1];
			else 
				sum = sum + lattice[tmpx][tmpy]*lattice[tmpx][lattice.size()-1];

			if(tmpx < lattice.size()-1)
				sum = sum + lattice[tmpx][tmpy]*lattice[tmpx+1][tmpy];
			else
				sum = sum + lattice[tmpx][tmpy]*lattice[0][tmpy];

			if(tmpy < lattice.size()-1)
				sum = sum + lattice[tmpx][tmpy]*lattice[tmpx][tmpy+1];
			else
				sum = sum + lattice[tmpx][tmpy]*lattice[tmpy][0];

			if (sum <= 0)
				lattice[tmpx][tmpy] = -1*lattice[tmpx][tmpy];
			else if(genrand64_real2() < isingEs[sum/2-1]){
			//cout << sum << " " << isingEs[sum/2-1] << "\n";
				lattice[tmpx][tmpy] = -1*lattice[tmpx][tmpy];
			}
		}
	}
}

double calcIntEnergy(){
	int tmpcount = 0;
	
	for(int i = 0; i < lattice.size(); i++){
		for(int j = 0; j < lattice.size(); j++){
			tmpcount = tmpcount + lattice[i][j]*lattice[(i+1)%(lattice.size())][j]
				+ lattice[i][j]*lattice[i][(j+1)%(lattice.size())];
		}
	}

	return -1.0*tmpcount;
}

double calcMagnetization(){
	int tmpcount = 0;
	
	for(int i = 0; i < lattice.size(); i++)
		for(int j = 0; j < lattice.size(); j++)
			tmpcount = tmpcount + lattice[i][j];

	return abs(tmpcount);
}

int main(){
	unsigned long long seed2 = 43534095836234;
	init_genrand64(seed2);
	
	double temperature, prevH = 0;
	int latticesize = 16;
	double tempstep = 0.05;
	double intH,mag,magsq = 0;
	double tmp = 0;
	double normalize = 1.0/(latticesize*latticesize);
	lattice.resize(latticesize);
	
	isingEs.resize(2);
	
	stringstream stst;
	stst << latticesize;
	string s = "c_v.ising";
	s = s.append(stst.str());
	myfile.open(s.c_str());
	
	for(int i = 0; i < lattice.size(); i++){
		lattice[i].resize(latticesize);
		for(int j = 0; j < lattice.size(); j++){
			if(genrand64_real2()<0.5)
				lattice[i][j] = -1;
			else
				lattice[i][j] = 1;
		}
	}
	
	tmp = 1.0/10000.0;
	
	for(double temp = 1.0; temp < 4.0; temp=temp+0.05){
		temperature = temp;
		isingEs[0] = exp(-4.0/temperature);
		isingEs[1] = exp(-8.0/temperature);
		intH = 0;
		mag = 0;
		magsq = 0;
		

		for(int i = 0; i < 10000; i++){
			mc_step();
			intH = intH + calcIntEnergy()*normalize;
			//tmp = calcIntEnergy()/temp;
			//mag = mag + tmp*normalize;
			//magsq = magsq + tmp*tmp*normalize*normalize;
		}
		intH = intH+tmp;
		abs(intH-prevH)/
		//cout << temperature << " " << tmp/(10000*lattice.size()*lattice.size())<<"\n";
		myfile << temperature << " " << magsq <<"\n";
	}
}