// Now add some reshuffling stuff
#include <iostream>
#include <algorithm>  // std::random_shuffle
#include <fstream>
#include <string>
#include <iomanip>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <ctime>
#include <cstdlib>      // std::rand, std::srand
#include "MyStatistics.h"

// random generator function:
int myrandom (int i) { return std::rand()%i;}

int main(int argc, char *argv[]){

//std::srand ( unsigned ( std::time(0) ) );

std::cout << "argc = " << argc << std::endl;
for (int i=0; i<argc;++i){
    std::cout << "argv [ " << i << "] = " << argv[i] << std::endl;
}

int NDATA;
NDATA = atoi(argv[1]);
std::cout << "NDATA set to " << NDATA << std::endl;

int NMC;
NMC = atoi(argv[2]);
std::cout << "NMC set to " << NMC << std::endl;

int NPERMS;
NPERMS = atoi(argv[3]);
std::cout << "NPERMS set to " << NPERMS << std::endl;

unsigned int FIRSTSEED;
FIRSTSEED = atoi(argv[4]);
std::cout << "FIRSTSEED set to " << FIRSTSEED << std::endl;

std::srand(FIRSTSEED);
int random_variable = std::rand();
std::cout << "First random value on [0 " << RAND_MAX << "]: " 
          << random_variable << '\n';

std::cout << "int\t"
              << std::numeric_limits<int>::lowest() << '\t'
              << std::numeric_limits<int>::max() << '\n';

int start_time = clock();

std::vector<std::pair<double,double>> vdata,vmc; 

std::cout << "Max elements that can be inserted into a vector of doubles having elements of size '" 
          << sizeof( std::vector<double> ) << "' is: "
          << std::vector<double>().max_size() << std::endl;

TFile* f = (TFile*) new TFile("histos.root","recreate");

// Define some histograms
TH1D* histdata = (TH1D*) new TH1D("histdata", "histdata", 1100, 0.0, 1.1);
TH1D* histmc = (TH1D*) new TH1D("histmc", "histmc", 1100, 0.0, 1.1);
TH1D* h = (TH1D*) new TH1D("h", "h", 1000, -1.15, -1.05);

std::ifstream inputDataFile("datafile.cdat");
if (inputDataFile) {
    int nheader;
    inputDataFile >> nheader;
    double x1,x2;
    while ( inputDataFile >> x1 >> x2 ) {
        vdata.push_back(std::make_pair(x1,x2));
    }
}
inputDataFile.close();

std::ifstream inputMCFile("mcfile.cdat");
if (inputMCFile) {
    int nheader;
    inputMCFile >> nheader;
    double x1,x2;
    while ( inputMCFile >> x1 >> x2 ) {
        vmc.push_back(std::make_pair(x1,x2));
    }
}
inputMCFile.close();

std::cout << "Available data vector of pairs has size " << vdata.size()<< std::endl;
std::cout << "Available MC vector of pairs has size " << vmc.size() << std::endl;

// Iterate over each vector and fill histogram with scaled center-of-mass energy
std::vector<std::pair<double,double>>::const_iterator it;
for (it = vdata.begin(); it!=vdata.end(); ++it){
   std::pair<double,double> thispair = *it;
   double x1 = thispair.first;
   double x2 = thispair.second;
   histdata->Fill(sqrt(x1*x2));
}
for (it = vmc.begin(); it!=vmc.end(); ++it){
   std::pair<double,double> thispair = *it;
   double x1 = thispair.first;
   double x2 = thispair.second;
   histmc->Fill(sqrt(x1*x2));
}

// Form pooled data-set consisting of NDATA+NMC events. Take first NDATA events and NMC events from each
std::vector<std::pair<double,double>> vpool;

for (it = vdata.begin(); it != vdata.begin() + NDATA; ++it){
   vpool.push_back(*it);
}
for (it = vmc.begin(); it != vmc.begin() + NMC; ++it){
   vpool.push_back(*it);
}
std::cout << "Chosen pooled vector of pairs has size " << vpool.size()<< std::endl;

// Now calculate some statistic for the sample that addresses the question of 
// whether the distribution of the pairs labelled as data is consistent with the 
// distribution of pairs labelled as MC
// For starters let's look at <xdata> where we use sqrt(x1*x2) as 
// the quantity of interest and restrict to the actual data events.

double Tobs = MyStatisticECM(NDATA,vpool);

// Now do some shuffling. Let's get started using built in random number generator. 
// See http://www.cplusplus.com/reference/algorithm/random_shuffle/

int ntail = 0;
for (int i=0; i<NPERMS; ++i){
   if(i%100 ==0)std::cout << "Permutation " << i << std::endl;
// FIXME - more control over the randomness preferred. 

// Use built-in random generator  
//   std::random_shuffle ( vpool.begin(), vpool.end() );
   
// Use myrandom above seeded by FIRSTSEED
   std::random_shuffle ( vpool.begin(), vpool.end(), myrandom );
      
   double Ti = MyStatisticECM(NDATA,vpool);
   h->Fill(Ti);
   if(Ti >= Tobs)ntail +=1;  // Count events with statistic exceeding the correct data/MC partition.
}

std::cout << "Tobs: Mean scaled sqrt(s) difference (in per cent) of the data events " << std::fixed 
          << std::setprecision(12) << std::setw(16) <<Tobs << std::endl;


// See p518 of Bootstrap: Tim Hesterberg.
double pvalue = double(ntail+1)/double(NPERMS+1);

std::cout << "Empirical p-value (high tail probability) of " << pvalue 
          << " based on " << ntail << " of the " << NPERMS 
          << " permutations " << std::endl; 

// Define the Canvas
TCanvas *c1 = new TCanvas("c1", "canvas", 800, 600);
histdata->Draw();
histmc->Draw();
h->Draw();
f->Write();

int stop_time = clock();
std::cout << "Execution time (s): " << double(stop_time-start_time)/double(CLOCKS_PER_SEC) << std::endl;

return 0;
}
