//
// Make a simple program to read in one file 
// and spit out a randomized sub-set of the same file 
// using some of the existing infrastructure.
//
// This will make it easier to do some meaningful 
// commissioning/timing tests for the computationally intensive statistics.
// Especially needed since the GP files were sorted by collision time.
//
//            Graham W. Wilson    30-DEC-2021
//
#include <iostream>
#include <algorithm>  // std::random_shuffle
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cstdlib>      // std::rand, std::srand

// random generator function:
int myrandom (int i) { return std::rand()%i;}

int main(int argc, char *argv[]){

// Deal with input arguments
   std::cout << "argc = " << argc << std::endl;
   for (int i=0; i<argc;++i){
       std::cout << "argv [ " << i << "] = " << argv[i] << std::endl;
   }

   int NDATA = atoi(argv[1]);
   std::cout << "NDATA set to " << NDATA << std::endl;

   unsigned int FIRSTSEED = atoi(argv[2]);
   std::cout << "FIRSTSEED set to " << FIRSTSEED << std::endl;
   std::srand(FIRSTSEED);
   int random_variable = std::rand();
   std::cout << "First random value on [0 " << RAND_MAX << "]: " 
             << random_variable << '\n';

   int start_time = clock();

   std::vector<std::pair<double,double>> vdata;

// Define input data set
   std::ifstream inputDataFile("MasterDataSet.cdat");
   if (inputDataFile) {
       int nheader;
       inputDataFile >> nheader;
       double x1,x2;
       while ( inputDataFile >> x1 >> x2 ) {
          vdata.push_back(std::make_pair(x1,x2));
       }
   }
   inputDataFile.close();

   std::cout << "Available data vector of pairs has size " 
             << vdata.size()<< std::endl;

// Now randomize the complete data vector using the usual shuffle
// Use myrandom above seeded by FIRSTSEED
   std::random_shuffle ( vdata.begin(), vdata.end(), myrandom );

// Now iterate over the first NDATA elements of the shuffled vector 
// writing them out to a file
   std::ofstream outputDataFile("Subset.cdat");
   outputDataFile << NDATA << std::endl;          // The header part
   std::vector<std::pair<double,double>>::const_iterator it;
   for (it = vdata.begin(); it!=vdata.begin()+NDATA; ++it){
      std::pair<double,double> thispair = *it;
      double x1 = thispair.first;
      double x2 = thispair.second;
      outputDataFile << std::fixed << std::setprecision(9) << x1 << " " << x2 << std::endl;
   }
   outputDataFile.close();

   int stop_time = clock();
   std::cout << "Execution time (s): " 
             << double(stop_time-start_time)/double(CLOCKS_PER_SEC) 
             << std::endl;

   return 0;
}
