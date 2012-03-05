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
ofstream c_v_diff,c_v, susc, intE, magf;

void mc_step(){
	for(int i = 0; i < 1000*lattice.size()*lattice.size(); i++){
			int tmpx = (int)(genrand64_real2()*lattice.size());
			int tmpy = (int)(genrand64_real2()*lattice.size());
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
				lattice[tmpx][tmpy] = -1*lattice[tmpx][tmpy];
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
	//initialization of the random number generator
	unsigned long long seed2 = 43534095836234;
	init_genrand64(seed2);
	
	//important simulation variables
	int latticesize = 64;
	int numsamples  = 500;
	int nummcsteps  = 5000;
	double tempstep = 0.125;
	
	//internal temporary terms for each set of monte carlo steps
	double intH, intHsq, cv, mag, magsq, cvfluc, cvflucsq = 0;
	//external sum terms that represent sampling around the mean value
	double intHvar, prevHvar, magvar = 0;
	//temporary variables
	double tmp,tmpm,tmpie, tmpmag, tmpcv, prevH, prevHsq = 0;
	//normalization variable for the lattice size
	double normalize = 1.0/(latticesize*latticesize);
	
	//resizing matrices
	lattice.resize(latticesize);
	isingEs.resize(2);
	for(int i = 0; i < lattice.size(); i++)
		lattice[i].resize(latticesize);
	
	//Preparing the files for IO
	stringstream stst;
	stst << latticesize;
	string s = "intE.";
	intE.open((s.append(stst.str())).c_str());
	s = "mag.";
	magf.open((s.append(stst.str())).c_str());
	s = "c_v.";
	c_v.open((s.append(stst.str())).c_str());
	s = "susc.";
	susc.open((s.append(stst.str())).c_str());
	s = "c_v.diff.";
	c_v_diff.open((s.append(stst.str())).c_str());
	
	//looping over all temperatures of interest
	for(double temp = 1.0; temp < 4.0; temp=temp+tempstep){
		//setting up the energy here so that all we have to do is
		//look up a value. 
		isingEs[0] = exp(-4.0/temp);
		isingEs[1] = exp(-8.0/temp);
		
		intH = 0;
		intHsq = 0;
		mag = 0;
		magsq = 0;
		
		tmp = 1.0/10000.0;
		
		//temp variables regarding the calculation of magnetization and
		//internal energy
		tmpmag = 0;
		tmpie = 0;
		
		tmpm = 0;
		
		for(int i = 0; i < lattice.size(); i++){
			for(int j = 0; j < lattice.size(); j++){
				if(genrand64_real2()<0.5)
					lattice[i][j] = -1;
				else
					lattice[i][j] = 1;
			}
		}
//		cout << temp << "\n";
		
		for(int j = 0; j < numsamples; j++){
				mc_step();
				
				tmpie = calcIntEnergy()*normalize;
				tmpmag = calcMagnetization()*normalize;
				//We want to calculate the internal energy, the magnetization, the susceptibility from fluctuations
				//the specific heat from fluctuations, and the specific heat from differentiating the internal
				//energy. 
				intH = intH + tmpie;
				intHsq = intHsq + tmpie*tmpie;
				
				mag = mag + tmpmag;
				magsq = magsq + tmpmag*tmpmag;
				
		}
		
		if(temp == 1.0){
			prevH = intH;
			prevHsq = intHsq;
		}
		tmp = fabs(intH-prevH)/(1.0*numsamples*tempstep);
		intHvar  = (intHsq-(intH*intH)/(1.0*numsamples))/(numsamples*numsamples);
		prevHvar = (prevHsq-(prevH*prevH)/(1.0*numsamples))/(numsamples*numsamples);
		magvar   = (magsq-(mag*mag)/(1.0*numsamples))/(numsamples*numsamples);
		
		prevH    = intH;
		prevHsq  = intHsq;
		
		intE << temp << " " << intH/(1.0*numsamples) << " " << sqrt(intHvar) <<"\n";
		magf << temp << " " << mag/(1.0*numsamples)  << " " << sqrt(magvar) << "\n";
		susc << temp << " " << magvar  << "\n";
		c_v  << temp << " " << intHvar << "\n"; 
		
		c_v_diff << temp << " " << tmp << "\n";
	}
}
