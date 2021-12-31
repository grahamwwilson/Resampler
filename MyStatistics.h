#include <TMath.h>

double MyStatisticECM(const int NDATA, std::vector<std::pair<double,double>>& vpool);
double MyStatisticEdiff(const int NDATA, std::vector<std::pair<double,double>>& vpool);
double MyTwoSampleKSTest(const int NDATA, const int NMC, std::vector<double>& v1, std::vector<double>& v2);
double MyPooledTwoSampleKSTest(const int n1, const int n2, std::vector<double>& v);
double MyPooledTwoSampleKSTest(const int itype, const int n1, const int n2, std::vector<std::pair<double,double>>& v);
double MyEnergyStatistic(const int N, std::vector<std::pair<double,double>>& v);

double MyEnergyStatistic(const int N, std::vector<std::pair<double,double>>& v){

// First separate the pooled vector into the two separate ones of 
// length N for data and of length M for MC
std::vector<std::pair<double,double>> v1,v2;
v1.assign(v.begin(), v.begin()+N);
v2.assign(v.begin()+N, v.end());
unsigned int M = unsigned(v2.size());

// Implement equation 3.1 of Aslan and Zech NIM paper.
// "Statistical energy as a tool for binning-free, multivariate 
//  goodness-of-fit tests, two-sample comparison and unfolding"
// NIM A537 (2005) 626-636.
//
// This is rather nasty in terms of computational complexity
// The xxsum loop has N(N-1)/2 summands
// The xysum part has NM summands
// So in total N(M + (N-1)/2 )

std::vector<std::pair<double,double>>::const_iterator it1;
std::vector<std::pair<double,double>>::const_iterator it2;
std::vector<std::pair<double,double>>::const_iterator it3;

// Try Gaussian distance function
const double RMS = 0.005;                 // Try 0.5%
const double FACTOR = -0.5/(RMS*RMS);
double value,valuexx,valuexy;

double xxsum = 0.0;
double xysum = 0.0;
int icount=0;
for (it1 = v1.begin(); it1!= v1.end(); ++it1) {    //Hopefully this still works.
    std::pair<double,double> pair1 = *it1;
    double x11 = pair1.first;
    double x21 = pair1.second;
    icount +=1;
    if(icount%10000==0)std::cout << "Loop " << icount << " in MyEnergyStatistic " << std::endl;
    for (it2 = it1+1; it2!= v1.end(); ++it2) {
        std::pair<double,double> pair2 = *it2;
//        double dx1 = pair1.first - pair2.first;     // x1 distance
//        double dx2 = pair1.second - pair2.second;   // x2 distance
        double dx1 = x11 - pair2.first;     // x1 distance
        double dx2 = x21 - pair2.second;    // x2 distance        
        double weight = exp(FACTOR*(dx1*dx1 + dx2*dx2));
        xxsum += weight;
    }
    for (it3 = v2.begin(); it3!= v2.end(); ++it3) {
        std::pair<double,double> pair3 = *it3;
//        double dx1 = pair1.first - pair2.first;     // x1 distance
//        double dx2 = pair1.second - pair2.second;   // x2 distance
        double dx1 = x11 - pair3.first;     // x1 distance
        double dx2 = x21 - pair3.second;    // x2 distance        
        double weight = exp(FACTOR*(dx1*dx1 + dx2*dx2));
        xysum += weight;
    }    
}

valuexx = xxsum/(double(N)*double(N));
valuexy = -xysum/(double(N)*double(M));
value = valuexx + valuexy;
std::cout << "MyEnergyStatistic evaluates to " << value << " xx: " << valuexx << " xy: " << valuexy << std::endl;
 
return value; 
}

double MyStatisticECM(const int NDATA, std::vector<std::pair<double,double>>& vpool){

// Now calculate some statistic for the sample that addresses the question of 
// whether the distribution of the pairs labelled as data is consistent with the 
// distribution of pairs labelled as MC
// For starters let's look at <xdata> where we use sqrt(x1*x2) as 
// the quantity of interest and restrict to the actual data events.

int ncheck = 0;
double T=0.0;
std::vector<std::pair<double,double>>::const_iterator it;
for (it = vpool.begin(); it!= vpool.begin() + NDATA; ++it){
   std::pair<double,double> thispair = *it;
   double x1 = thispair.first;
   double x2 = thispair.second;
   T += sqrt(x1*x2);
   ncheck +=1;
}
T = T/double(NDATA);
T = 100.0*(T - 1.0);  // difference from 1.0 in per cent units

std::cout << "Mean ECM offset (in %) for data labels " << std::fixed 
          << std::setprecision(12) << std::setw(16) <<T << " based on " << ncheck << " events " << std::endl;
          
return T;
}

double MyStatisticEdiff(const int NDATA, std::vector<std::pair<double,double>>& vpool){

// Now calculate some statistic for the sample that addresses the question of 
// whether the distribution of the pairs labelled as data is consistent with the 
// distribution of pairs labelled as MC
// This time we look at <pZ> in scaled units as the quantity of interest and restrict to the actual data events.

int ncheck = 0;
double T=0.0;
std::vector<std::pair<double,double>>::const_iterator it;
for (it = vpool.begin(); it!= vpool.begin() + NDATA; ++it){
   std::pair<double,double> thispair = *it;
   double x1 = thispair.first;
   double x2 = thispair.second;
   T += (x1-x2);
   ncheck +=1;
}
T = T/double(NDATA);
T = 100.0*(T - 0.0);  // difference from 0.0 in per cent units

std::cout << "Mean scaled Ediff (in %) for data labels " << std::fixed 
          << std::setprecision(12) << std::setw(16) <<T << " based on " << ncheck << " events " << std::endl;
          
return T;
}

double MyTwoSampleKSTest(std::vector<double>& v1, std::vector<double>& v2){

// Similar code to kstests.h in NR3, p738

unsigned int n1 = unsigned(v1.size());
unsigned int n2 = unsigned(v2.size());

std::sort(v1.begin(),v1.end());
std::sort(v2.begin(),v2.end());

double en1 = double(n1);
double en2 = double(n2);
double fn1 = 0.0;
double fn2 = 0.0;
double d = 0.0;
int j1 = 0;
int j2 = 0;
double d1,d2,dt;
double en;

while (j1 < n1 && j2 < n2) {
   if ( ( d1 = v1[j1] ) <= ( d2 = v2[j2] ) )
       do
          fn1 = ++j1/en1;
       while ( j1 < n1 && d1 == v1[j1]);
   if ( d2 <= d1 )
       do
          fn2 = ++j2/en2;
       while ( j2 < n2 && d2 == v2[j2]);
   if ( (dt=abs(fn2-fn1)) > d) d = dt;     
}  
en = sqrt(en1*en2/(en1+en2));
double dp = d*(0.12 + en + (0.11/en));   // Finite approximation
double z = en*d;                         // Asymptotic case
double pvalue1 = TMath::KolmogorovProb(z);
double pvalue2 = TMath::KolmogorovProb(dp);

std::cout << "Sample sizes:                " << n1 << " " << n2 << std::endl;
std::cout << "Effective number of points:  " << en*en << std::endl;
std::cout << "KS Test results. Dobs:       " << d << std::endl;
std::cout << "Pure z:                      " << z << std::endl;
std::cout << "Stephens T*:                 " << dp << std::endl;
std::cout << "Asymptotic p-value           " << pvalue1 << std::endl;
std::cout << "Finite approximation p-value " << pvalue2 << std::endl;

return pvalue2; 

}

double MyPooledTwoSampleKSTest(const int n1, const int n2, std::vector<double>& v){

// Similar code to kstests.h in NR3, p738

// Copy first n1 entries of v into v1 
// and last n2 entries of v into v2
// Assumes that v.size() = n1+n2
// Try using assign method highlighted here https://www.geeksforgeeks.org/ways-copy-vector-c/

double pvalue2;
unsigned int N = unsigned(v.size());

if(N == (n1+n2)){

std::vector<double> v1,v2;
v1.assign(v.begin(), v.begin()+n1);
v2.assign(v.begin()+n1, v.end());

// Do some spot checks
std::cout << " v  size " << N << std::endl;
std::cout << " v1 size " << v1.size() << std::endl;
std::cout << " v2 size " << v2.size() << std::endl;
std::cout << " v[0] " << v[0] << std::endl;
std::cout << " v[n1-1] " << v[n1-1] << std::endl;
std::cout << " v[n1] " << v[n1] << std::endl;
std::cout << " v[n1+n2-1] " << v[n1+n2-1] << std::endl;
std::cout << " v1[0] " << v1[0] << std::endl;
std::cout << " v1[n1-1] " << v1[n1-1] << std::endl;
std::cout << " v2[0] " << v2[0] << std::endl;
std::cout << " v2[n2-1] " << v2[n2-1] << std::endl;

std::sort(v1.begin(),v1.end());
std::sort(v2.begin(),v2.end());

std::cout << "after sorts " << std::endl;
std::cout << " v1[0] " << v1[0] << std::endl;
std::cout << " v1[n1-1] " << v1[n1-1] << std::endl;
std::cout << " v2[0] " << v2[0] << std::endl;
std::cout << " v2[n2-1] " << v2[n2-1] << std::endl;

double en1 = double(n1);
double en2 = double(n2);
double fn1 = 0.0;
double fn2 = 0.0;
double d = 0.0;
int j1 = 0;
int j2 = 0;
double d1,d2,dt;
double en;

while (j1 < n1 && j2 < n2) {
   if ( ( d1 = v1[j1] ) <= ( d2 = v2[j2] ) )
       do
          fn1 = ++j1/en1;
       while ( j1 < n1 && d1 == v1[j1]);
   if ( d2 <= d1 )
       do
          fn2 = ++j2/en2;
       while ( j2 < n2 && d2 == v2[j2]);
   if ( (dt=abs(fn2-fn1)) > d) d = dt;     
}  

en = sqrt(en1*en2/(en1+en2));
double dp = d*(0.12 + en + (0.11/en));   // Finite approximation
double z = en*d;                         // Asymptotic case
double pvalue1 = TMath::KolmogorovProb(z);
pvalue2 = TMath::KolmogorovProb(dp);

std::cout << "Sample sizes:                " << n1 << " " << n2 << std::endl;
std::cout << "Effective number of points:  " << en*en << std::endl;
std::cout << "KS Test results. Dobs:       " << d << std::endl;
std::cout << "Pure z:                      " << z << std::endl;
std::cout << "Stephens T*:                 " << dp << std::endl;
std::cout << "Asymptotic p-value           " << pvalue1 << std::endl;
std::cout << "Finite approximation p-value " << pvalue2 << std::endl;
}
else{
  std::cout << "PROBLEM with pooled KS test " << N << " " << n1 << " " << n2 << std::endl;
  pvalue2 = -999.0;   // Flag problems
}

return pvalue2;  

}

double MyPooledTwoSampleKSTest(const int itype, const int n1, const int n2, std::vector<std::pair<double,double>>& v){

// Implement in a similar way to EnergyStatistic etc using the pooled sample, v, and using the pairs.
// itype = 1 => ECM
// itype = 2 => Ediff
// This is if anything a bit more inefficient, but is more easily integrated with the other methods

// Similar code to kstests.h in NR3, p738

// Copy first n1 entries of v into v1 
// and last n2 entries of v into v2
// Assumes that v.size() = n1+n2
// Try using assign method highlighted here https://www.geeksforgeeks.org/ways-copy-vector-c/

double pvalue2;
unsigned int N = unsigned(v.size());

if(N == (n1+n2)){

std::vector<std::pair<double,double>> p1,p2;
p1.assign(v.begin(), v.begin()+n1);
p2.assign(v.begin()+n1, v.end());

std::vector<double> v1,v2;
std::vector<std::pair<double,double>>::const_iterator it;
for (it = p1.begin(); it!=p1.end(); ++it){
   std::pair<double,double> thispair = *it;
   double x1 = thispair.first;
   double x2 = thispair.second;
   if(itype == 1){
      v1.push_back(sqrt(x1*x2));
   }
   else{
      v1.push_back(x1-x2);   
   }
}
for (it = p2.begin(); it!=p2.end(); ++it){
   std::pair<double,double> thispair = *it;
   double x1 = thispair.first;
   double x2 = thispair.second;
   if(itype == 1){
      v2.push_back(sqrt(x1*x2));
   }
   else{
      v2.push_back(x1-x2);   
   }   
}

std::sort(v1.begin(),v1.end());
std::sort(v2.begin(),v2.end());

std::cout << "after sorts " << std::endl;
std::cout << " v1[0] " << v1[0] << std::endl;
std::cout << " v1[n1-1] " << v1[n1-1] << std::endl;
std::cout << " v2[0] " << v2[0] << std::endl;
std::cout << " v2[n2-1] " << v2[n2-1] << std::endl;

double en1 = double(n1);
double en2 = double(n2);
double fn1 = 0.0;
double fn2 = 0.0;
double d = 0.0;
int j1 = 0;
int j2 = 0;
double d1,d2,dt;
double en;

while (j1 < n1 && j2 < n2) {
   if ( ( d1 = v1[j1] ) <= ( d2 = v2[j2] ) )
       do
          fn1 = ++j1/en1;
       while ( j1 < n1 && d1 == v1[j1]);
   if ( d2 <= d1 )
       do
          fn2 = ++j2/en2;
       while ( j2 < n2 && d2 == v2[j2]);
   if ( (dt=abs(fn2-fn1)) > d) d = dt;     
}  

en = sqrt(en1*en2/(en1+en2));
double dp = d*(0.12 + en + (0.11/en));   // Finite approximation
double z = en*d;                         // Asymptotic case
double pvalue1 = TMath::KolmogorovProb(z);
pvalue2 = TMath::KolmogorovProb(dp);

std::cout << "Sample sizes:                " << n1 << " " << n2 << std::endl;
std::cout << "Effective number of points:  " << en*en << std::endl;
std::cout << "KS Test results. Dobs:       " << d << std::endl;
std::cout << "Pure z:                      " << z << std::endl;
std::cout << "Stephens T*:                 " << dp << std::endl;
std::cout << "Asymptotic p-value           " << pvalue1 << std::endl;
std::cout << "Finite approximation p-value " << pvalue2 << std::endl;
}
else{
  std::cout << "PROBLEM with pooled KS test " << N << " " << n1 << " " << n2 << std::endl;
  pvalue2 = -999.0;   // Flag problems
}

return pvalue2;  

}
