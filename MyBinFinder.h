// MyBinFinder.h

int MyBinFinder(double x, std::vector<double>& bins);
int PairBin(std::pair<double,double> p, std::vector<double>& binsx1, std::vector<double>& binsx2, int icase=0);
int X1Bin(std::pair<double,double> p, std::vector<double>& binsx1);
int X2Bin(std::pair<double,double> p, std::vector<double>& binsx2);

// Given value x, find the corresponding bin number, using C++ indexing 
// convention (0-99) for 100 bins.
//
// The bin boundaries were defined 
// by functions in BeamSpectrumGenerator/findbin.f
// The first 99 values are bin upper boundaries. 
// The last one is the largest value observed - so shouldn't be applied
// So the bins are (-inf, x0] (x0, x1], (x1, x2], ....  (x97, x98], (x98, inf)  
int MyBinFinder(double x, std::vector<double>& bins){

    int nbins = int(bins.size());
    
    int ibin = nbins-1;  // Set default as last bin index for the (x98, inf) bin
                         // which is not tested in the following loop.
    
// This could be done more efficiently using binary search
// with complexity of O(log N) instead of O(N).
// Remember the bins vector by design is already monotonically increasing 
// and therefore sorted.

    for(int i=0; i<nbins-1; ++i){
        if( x <= bins[i] ){
           ibin = i;
           return ibin;
        }
    }
    
    return ibin;
    
}

// Three higher level methods using the (X1,X2) pair information

int PairBin(std::pair<double,double> p, std::vector<double>& binsx1, std::vector<double>& binsx2, int icase=0){
    auto x1 = p.first;
    auto x2 = p.second;
    int ibinx1 = MyBinFinder(x1, binsx1);
    int ibinx2 = MyBinFinder(x2, binsx2);
    int pairbin;
    if(icase ==0 ){
// This is the convention previously used
       pairbin = 100*ibinx1 + ibinx2;
    }
    else{
       pairbin = 100*ibinx2 + ibinx1;
    }
// Note we could even adopt more of a 2-d -> 1-d mapping that 
// preserves better the concept of locality.   
    
    return pairbin;
}

int X1Bin(std::pair<double,double> p, std::vector<double>& binsx1){
    auto x1 = p.first;
    int ibinx1 = MyBinFinder(x1, binsx1);
    return ibinx1;
}

int X2Bin(std::pair<double,double> p, std::vector<double>& binsx2){
    auto x2 = p.second;
    int ibinx2 = MyBinFinder(x2, binsx2);
    return ibinx2;
}
