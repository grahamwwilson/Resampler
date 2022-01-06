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

typedef std::mt19937 RandomNumberGenerator;

#include "MyOmp.h"                // OpenMP
#include "EstimatorDefinitions.h" // Estimator enum, EstimatorNames and string->enum conversion function
#include "MyFileHandler.h"        // handle (x1,x2) .cdat files and xibin edge files
#include "MyBinFinder.h"          // assignment of (x1,x2) values to bins
#include "MyStatistics.h"

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

unsigned long int myseed;
myseed = atoi(argv[4]);
std::cout << "myseed set to " << myseed << std::endl;

std::string ESTIMATORCHOICE = argv[5];
std::cout << "ESTIMATORCHOICE " << ESTIMATORCHOICE << std::endl;
Estimator estimator = convert(ESTIMATORCHOICE);
std::cout << "Estimator enum set to " << estimator << std::endl;

int start_time = clock();   // Not so useful with multithreading ...

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
auto vdata = CreateX1X2PairVector("dataset1.cdat");
auto vmc = CreateX1X2PairVector("dataset2.cdat");
auto vdataECM = CreateECMVector(vdata);
auto vdataEdiff = CreateEdiffVector(vdata);
auto vdataX1 = CreateX1Vector(vdata);
auto vdataX2 = CreateX2Vector(vdata);
auto vmcECM = CreateECMVector(vmc);
auto vmcEdiff = CreateEdiffVector(vmc);
auto vmcX1 = CreateX1Vector(vmc);
auto vmcX2 = CreateX2Vector(vmc);
auto binsx1 = CreateBinVector("x1bins-100-1M-1-0.dat");
auto binsx2 = CreateBinVector("x2bins-100-1M-1-0.dat");

std::cout << "Available data vector of pairs has size " << vdata.size()<< std::endl;
std::cout << "Available MC vector of pairs has size " << vmc.size() << std::endl;
std::cout << "x1 bin vector of size " << binsx1.size() << std::endl;
std::cout << "x2 bin vector of size " << binsx2.size() << std::endl;

// Iterate over each vector and fill histogram with scaled center-of-mass energy
for (auto it = vdata.begin(); it!=vdata.end(); ++it){
   auto thispair = *it;
   auto x1 = thispair.first;
   auto x2 = thispair.second;
   histdata->Fill(sqrt(x1*x2));
}
for (auto it = vmc.begin(); it!=vmc.end(); ++it){
   auto thispair = *it;
   auto x1 = thispair.first;
   auto x2 = thispair.second;
   histmc->Fill(sqrt(x1*x2));
}

// Conduct two-sample KS tests (use all available events in samples)
std::cout << " " << std::endl;
std::cout << "Unbinned classic 2-sample KS Test of 1-d X1 distribution " << std::endl;
double pKSX1 = MyTwoSampleKSTest(vdataX1,vmcX1);
std::cout << " " << std::endl;
std::cout << "Unbinned classic 2-sample KS Test of 1-d X2 distribution " << std::endl;
double pKSX2 = MyTwoSampleKSTest(vdataX2,vmcX2);
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
vpool.assign(vdata.begin(), vdata.begin()+NDATA);                //Copy NDATA "DATA" elements to start of vpool vector 
vpool.insert(vpool.end(), vmc.begin(), vmc.begin() + NMC);       //Append NMC "MC" elements to end of vpool vector
std::cout << "Chosen pooled vector of pairs has size " << vpool.size()<< std::endl;
// For convenience also make X1 and X2 vectors of the pooled sample
auto vpoolX1 = CreateX1Vector(vpool);
auto vpoolX2 = CreateX2Vector(vpool);
auto pX1 = MyMoments(vpoolX1);
auto pX2 = MyMoments(vpoolX2);

// Now calculate some statistic for the sample that addresses the question of 
// whether the distribution for events labelled as data is consistent with the 
// distribution labelled as MC. 
// Current choices are defined by the EstimatorDefinitions.h include file.
// 
// We only need to do this for events labelled as data in the case of 
// the MeanECM, MeanEdiff implementations (similar to Good 2000 Chapter 1) petri-dish example.
//
// NOTE: Adopt convention of Williams that larger value of 
// each statistic labelled T, means worse agreement like a chi-squared value.
//
// Do the permutation test.
// We randomly shuffle the pooled vector of length (NDATA+NMC) and 
// call the first NDATA elements data.
// Now do some shuffling. srand stuff now deleted.


int nthreads;
int taskid;
std::vector<double> vResult;
vResult.reserve(NPERMS); 

#pragma omp parallel for

for (unsigned int i=0; i<NPERMS; ++i){
  
   double Ti;
   
// Also to avoid race conditions, we first copy the original vector
// This also protects against subsequent modification of the vector 
// in the statistics functions.
   auto vpoolcopy(vpool);   

   if( i!= 0) {

// For reproducibility, especially with multi-threading, reseed the RNG for each permutation.
      RandomNumberGenerator rng(myseed + i);

// Now we randomly shuffle the copy to get a permutation instance. 
// This is the C++11 shuffle rather than random_shuffle. 
// RNG used is the Mersenne-Twister
// https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector 
      std::shuffle ( std::begin(vpoolcopy), std::end(vpoolcopy), rng );
   }
   
// The unshuffled copy (i = 0) that is not affected by the above if clause 
// is the properly labelled DATA vs MC test.

// The shuffled copies are used to evaluate the distribution of the 
// desired statistic using the permutation test.
// I suspect histogramming is not thread safe - still need to test it.
 
   if(estimator == MeanECM){     
      Ti = MyMeanStatistic(1,NDATA,vpoolcopy);
   }
   else if (estimator == MeanEdiff){
      Ti = MyMeanStatistic(2,NDATA,vpoolcopy);   
   }
   else if (estimator == MeanX1){
      Ti = MyMeanStatistic(3,NDATA,vpoolcopy);   
   }
   else if (estimator == MeanX2){
      Ti = MyMeanStatistic(4,NDATA,vpoolcopy);   
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
   else if (estimator == KSX1){
      const int itype=3;
      Ti = 1.0 - MyPooledTwoSampleKSTest(itype,NDATA,NMC,vpoolcopy);
   }
   else if (estimator == KSX2){
      const int itype=4;
      Ti = 1.0 - MyPooledTwoSampleKSTest(itype,NDATA,NMC,vpoolcopy);
   }   
   else if (estimator == ChiSquaredX1){
      Ti = MyTwoSampleChisq(NDATA, vpoolcopy, binsx1, binsx2,1);
   }
   else if (estimator == ChiSquaredX2){
      Ti = MyTwoSampleChisq(NDATA, vpoolcopy, binsx1, binsx2,2);
   }
   else if (estimator == ChiSquaredX1X2){
      Ti = MyTwoSampleChisq(NDATA, vpoolcopy, binsx1, binsx2,3);
   }
   else if (estimator == ChiSquaredX1PX2){
      Ti = MyTwoSampleChisq(NDATA, vpoolcopy, binsx1, binsx2,4);
   }         
 
   vResult[i] = Ti;

   nthreads = omp_get_num_threads();
   taskid = omp_get_thread_num();
//   #pragma omp critical   
   { 
      std::cout << "Permutation " << i << " threads " << nthreads << " task " << taskid << " Ti " << Ti << std::endl;
      std::cout << " " << std::endl;
   }
}  // End of OpenMP parallel for

double Tobs = vResult[0];
std::cout << "Tobs: " <<  EstimatorNames[estimator] << " " << std::fixed 
          << std::setprecision(12) << std::setw(16) << Tobs << std::endl;

// Now count tail events for the p-value estimate
int ntail = 0;
for (unsigned int i=1; i<NPERMS; ++i){  // Intentionally exclude i=0 as that is the data observation
   if(vResult[i] >= Tobs)ntail +=1;     // Count events with statistic exceeding the correct data/MC partition.
} 

int ntrials = NPERMS-1;    // the first one was used only for the actual experiment
double pvalueEstimate = double(ntail)/double(ntrials); // This is the unbiased p-value estimate from ntrials permutations
double pvalueTest = double(ntail+1)/double(ntrials+1); // This p-value guarantees conformity with stat. testing orthodoxy.
// pvalueTest is preferred in the permutation test/randomization test literature since it satisfies 
// the General Validity Criterion stated in Randomization Tests, Edgington and Onghena (2007) p36 for decision making,
// "A statistical testing procedure is valid if, under the null hypothesis, 
//  the probability of a P-value as small as P is no greater than P, for any P" 
// To see how this matters, it is sufficient to consider the case of an exact test, where 
// the p-value associated with the observation must be at least 1/Nperms, 
// but in performing an ART (approximate randomization test) it is quite 
// feasible to measure a p-value of exactly zero if one does not 
// include the +1 numerator and denominator corrections.
// (I still don't truly appreciate the concern given that pvalueEstimate would be an unbiased estimate).
// Maybe the Noreen book will clarify this.
// Ah-hah, if one obtains a p-value of 0, then this contradicts the above, because the 
// probability of a p-value as small as 0 is NOT no greater than 0 given that it has actually occurred.
// (This could also be an artefact of an algorithm that does not allow for random resampling of the original 
//  permutation).

std::cout << "Test p-value (high tail probability) of " << pvalueTest 
          << " based on " << ntail << " pool permutations as significant as the observation " << std::endl;
std::cout << "from the " << ntrials 
          << " pool permutations (+ the observation)" << std::endl; 

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
