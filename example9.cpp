//
// Look into adding OpenMP functionality 
// to speed things up.
//
#include <iostream>
#include <random>
#include <algorithm>  // std::shuffle
#include <fstream>
#include <string>
#include <iomanip>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <ctime>
//#include <cstdlib>      // std::rand, std::srand
#include "MyStatistics.h"
typedef std::mt19937 RandomNumberGenerator;
//typedef std::ranlux24 RandomNumberGenerator;

#include "MyOmp.h"                // OpenMP
#include "EstimatorDefinitions.h" // Estimator enum, EstimatorNames and string->enum conversion function

int main(int argc, char *argv[]){

checkomp();   // Check whether OpenMP was included in the compilation

// Deal with input arguments
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

unsigned int NPERMS;
NPERMS = unsigned(atoi(argv[3]));
std::cout << "NPERMS set to " << NPERMS << std::endl;

unsigned int FIRSTSEED;
FIRSTSEED = atoi(argv[4]);
std::cout << "FIRSTSEED set to " << FIRSTSEED << std::endl;

std::string ESTIMATORCHOICE = argv[5];
std::cout << "ESTIMATORCHOICE " << ESTIMATORCHOICE << std::endl;

Estimator estimator = convert(ESTIMATORCHOICE);
std::cout << "Estimator enum set to " << estimator << std::endl;

int CHOICE=3;
std::cout << "CHOICE set to " << CHOICE << std::endl;
          
// Set up Mersenne-Twister based RNG
unsigned long int myseed = FIRSTSEED;
//RandomNumberGenerator rng(myseed);

int start_time = clock();   // Not so useful with multithreading ...

std::vector<std::pair<double,double>> vdata,vmc;
std::vector<double> vdataECM, vmcECM;
std::vector<double> vdataEdiff, vmcEdiff;

std::cout << "Max elements that can be inserted into a vector of doubles having elements of size '" 
          << sizeof( std::vector<double> ) << "' is: "
          << std::vector<double>().max_size() << std::endl;

// Define histograms and histogram file
TFile* f = (TFile*) new TFile("histos_Ediff.root","recreate");
TH1D* histdata = (TH1D*) new TH1D("histdata", "histdata", 1100, 0.0, 1.1);
TH1D* histmc = (TH1D*) new TH1D("histmc", "histmc", 1100, 0.0, 1.1);
TH1D* h = (TH1D*) new TH1D("h", "h", 1000, -1.15, -1.05);
TH1D* hpz = (TH1D*) new TH1D("hpz", "hpz", 1000, -0.05, 0.05);
TH1D* hKSpval = (TH1D*) new TH1D("hKSpval","hKSpval",1000,0.0,1.0);
TH1D* hKSqval = (TH1D*) new TH1D("hKSqval","hKSqval",1000,0.0,1.0);

// Define input data sets
std::ifstream inputDataFile("dataset1.cdat");
if (inputDataFile) {
    int nheader;
    inputDataFile >> nheader;
    double x1,x2;
    while ( inputDataFile >> x1 >> x2 ) {
        vdata.push_back(std::make_pair(x1,x2));
        vdataECM.push_back(sqrt(x1*x2));
        vdataEdiff.push_back(x1-x2);
    }
}
inputDataFile.close();

std::ifstream inputMCFile("dataset2.cdat");
if (inputMCFile) {
    int nheader;
    inputMCFile >> nheader;
    double x1,x2;
    while ( inputMCFile >> x1 >> x2 ) {
        vmc.push_back(std::make_pair(x1,x2));
        vmcECM.push_back(sqrt(x1*x2));
        vmcEdiff.push_back(x1-x2);        
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

// Conduct two-sample KS tests (use all available events in samples)
std::cout << " " << std::endl;
std::cout << "Unbinned classic 2-sample KS Test of 1-d ECM distribution " << std::endl;
double pKSECM = MyTwoSampleKSTest(vdataECM,vmcECM);
std::cout << " " << std::endl;
std::cout << "Unbinned classic 2-sample KS Test of 1-d Ediff distribution " << std::endl;
double pKSEdiff = MyTwoSampleKSTest(vdataEdiff,vmcEdiff);
std::cout << " " << std::endl;

// Form pooled data-set consisting of NDATA+NMC events. Take first NDATA events and then NMC events from each
// (Be careful about taking only a subset of GP files, as they are often sorted by collision time so will not be 
//  a representative sub-sample from the parent distribution.)
std::vector<std::pair<double,double>> vpool;
for (it = vdata.begin(); it != vdata.begin() + NDATA; ++it){
   vpool.push_back(*it);
}
// std::sort(vpool.begin(),vpool.end()); // Sort the data part ?
for (it = vmc.begin(); it != vmc.begin() + NMC; ++it){
   vpool.push_back(*it);
}
std::cout << "Chosen pooled vector of pairs has size " << vpool.size()<< std::endl;

// Now calculate some statistic for the sample that addresses the question of 
// whether the distribution for events labelled as data is consistent with the 
// distribution labelled as MC. 
// Current choices are defined by the EstimatorDefinitions.h include file.
// 
// We only need to do this for events labelled as data in the case of 
// the MeanECM, MeanEdiff implementations (similar to Good 2000 Chapter 1) petri-dish example.

double Tobs,Ti;    // NOTE: Adopt convention of Williams that larger value of 
                   // each statistic labelled T, means worse agreement like a chi-squared value.



bool isTobsAlreadyCalculated = false;

// Do the permutation test.
// We randomly shuffle the pooled vector of length (NDATA+NMC) and 
// call the first NDATA elements data.
// Now do some shuffling. srand stuff now deleted.

int ntail = 0;
unsigned long int nwaits = 0;

int nthreads;
int taskid;
int ntrials = 0;

#pragma omp parallel for reduction (+:ntail,nwaits,ntrials)

for (unsigned int i=0; i<NPERMS; ++i){
   
// Also to avoid race conditions, we first copy the original vector
// This also protects against subsequent modification of the vector 
// in the statistics functions.
   std::vector<std::pair<double,double>> vpoolcopy(vpool);   

   if( i!= 0) {

// For reproducibility, especially with multi-threading, reseed the RNG for each permutation.
     RandomNumberGenerator rng(myseed + i);

// Now we randomly shuffle the copy to get a permutation instance. 
// This is the C++11 shuffle rather than random_shuffle. 
// RNG used is the Mersenne-Twister
// https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector 
     std::shuffle ( std::begin(vpoolcopy), std::end(vpoolcopy), rng );
   
   }
   
// The unshuffled copy (i = 0) is the properly labelled DATA vs MC test.
// The shuffled copy is used to evaluate the distribution of the 
// desired statistic using the permutation test.
// I suspect histogramming is not thread safe - still need to test it.
 
   if(estimator == MeanECM){     
      Ti = MyStatisticECM(NDATA,vpoolcopy);
      h->Fill(Ti);
   }
   else if (estimator == MeanEdiff){
      Ti = MyStatisticEdiff(NDATA,vpoolcopy);   
      hpz->Fill(Ti);
   }
   else if (estimator == Energy){
      Ti = MyEnergyStatistic(NDATA,vpoolcopy);
   }
   else if (estimator == Energy2){
      Ti = MySecondEnergyStatistic(NDATA,vpoolcopy);
   }    
   else if (estimator == KSECM){
      const int itype=1;
      Ti = 1.0 - MyPooledTwoSampleKSTest(itype,NDATA,NMC,vpoolcopy);
   }
   else if (estimator == KSEdiff){
      const int itype=2;
      Ti = 1.0 - MyPooledTwoSampleKSTest(itype,NDATA,NMC,vpoolcopy);
   }
 
      
   if(i == 0){
      Tobs = Ti;
      isTobsAlreadyCalculated = true;
   }   

// Count number of iterations
// where the experimental analysis result (the Tobs computation) is not ready in time 

   if( isTobsAlreadyCalculated == false){
      std::cout << "Iteration " << i << " finished too fast " << std::endl;
      nwaits +=1;
      // Maybe also add some busy work here too ...
//      for(int j=0; j<10000; j++){
//         int k = j+1;
//      }
   }
   
// Twiddle our thumbs here waiting for first thread (i==0) to finish 
// i==0 has less to do (no shuffling) - but there is no guarantee that it will be first. 
//   while (isTobsAlreadyCalculated == false){
//       ;  // potentially infinite do nothing while clause ... waiting for the first thread to set this boolean to true
//   }   
   
   if(i != 0 && isTobsAlreadyCalculated == true){
      ntrials +=1;
      if(Ti >= Tobs)ntail +=1;  // Count events with statistic exceeding the correct data/MC partition.
   }   

   nthreads = omp_get_num_threads();
   taskid = omp_get_thread_num();
//   #pragma omp critical   
   { 
      std::cout << "Permutation " << i << " threads " << nthreads << " task " << taskid << std::endl;
      std::cout << " " << std::endl;
   }
}

std::cout << "nwaits " << nwaits << std::endl;

std::cout << "Tobs: " <<  EstimatorNames[estimator] << std::fixed 
          << std::setprecision(12) << std::setw(16) <<Tobs << std::endl;


// See p518 of Bootstrap: Tim Hesterberg.
double pvalue = double(ntail+1)/double(ntrials+1);

std::cout << "Empirical p-value (high tail probability) of " << pvalue 
          << " based on " << ntail << " of the " << ntrials 
          << " trial permutations " << std::endl; 

// Define the Canvas
TCanvas *c1 = new TCanvas("c1", "canvas", 800, 600);
histdata->Draw();
histmc->Draw();
h->Draw();
hpz->Draw();
f->Write();

int stop_time = clock();
std::cout << "Execution time (s): " << double(stop_time-start_time)/double(CLOCKS_PER_SEC) << std::endl;

return 0;
}
