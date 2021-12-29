#include <TMath.h>

double MyStatisticECM(const int NDATA, std::vector<std::pair<double,double>>& vpool);
double MyStatisticEdiff(const int NDATA, std::vector<std::pair<double,double>>& vpool);
double MyTwoSampleKSTest(const int NDATA, const int NMC, std::vector<double>& v1, std::vector<double>& v2);

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

std::cout << "Mean scaled sqrt(s) difference (in %) for data labels " << std::fixed 
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

std::cout << "Mean scaled energy difference (in %) for data labels " << std::fixed 
          << std::setprecision(12) << std::setw(16) <<T << " based on " << ncheck << " events " << std::endl;
          
return T;
}

double MyTwoSampleKSTest(const int n1, const int n2, std::vector<double>& v1, std::vector<double>& v2){
// Similar code to kstests.h in NR3, p738
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
