
double MyStatisticECM(const int NDATA, std::vector<std::pair<double,double>>& vpool);
double MyStatisticEdiff(const int NDATA, std::vector<std::pair<double,double>>& vpool);

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


