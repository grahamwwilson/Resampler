// MyFileHandler.h

std::vector<std::pair<double,double>> CreateX1X2PairVector(std::string filename);
std::vector<double> CreateECMVector(std::vector<std::pair<double,double>>& v);
std::vector<double> CreateEDiffVector(std::vector<std::pair<double,double>>& v);
std::vector<double> CreateX1Vector(std::vector<std::pair<double,double>>& v);
std::vector<double> CreateX2Vector(std::vector<std::pair<double,double>>& v);
std::vector<double> CreateBinVector(std::string filename);

std::vector<std::pair<double,double>> CreateX1X2PairVector(std::string filename){

// Open input data set
   std::ifstream inputDataFile(filename);

   std::vector<std::pair<double,double>> v;

   if (inputDataFile) {
       int nheader;
       inputDataFile >> nheader;
       double x1,x2;
       while ( inputDataFile >> x1 >> x2 ) {
           v.push_back(std::make_pair(x1,x2));
       }
   }
   inputDataFile.close();  
   return v;
}

std::vector<double> CreateBinVector(std::string filename){

// Open input file with bin values
   std::ifstream inputDataFile(filename);

   std::vector<double> v;

   if (inputDataFile) {
       int ibin;
       double xbin;
       while ( inputDataFile >> ibin >> xbin ) {
           v.push_back(xbin);
       }
   }
   inputDataFile.close();  
   return v;   

}

std::vector<double> CreateECMVector(std::vector<std::pair<double,double>>& v){

   std::vector<double> vout;
   std::vector<std::pair<double,double>>::const_iterator it;
   
   for (it = v.begin(); it!=v.end(); ++it){
       std::pair<double,double> thispair = *it;
       double x1 = thispair.first;
       double x2 = thispair.second;
       vout.push_back(sqrt(x1*x2));
   }
   return vout;
}

std::vector<double> CreateEdiffVector(std::vector<std::pair<double,double>>& v){

   std::vector<double> vout;
   std::vector<std::pair<double,double>>::const_iterator it;
   
   for (it = v.begin(); it!=v.end(); ++it){
       std::pair<double,double> thispair = *it;
       double x1 = thispair.first;
       double x2 = thispair.second;
       vout.push_back(x1-x2);
   }
   return vout;    
}

std::vector<double> CreateX1Vector(std::vector<std::pair<double,double>>& v){

   std::vector<double> vout;
   for (auto it = v.begin(); it!=v.end(); ++it){
       std::pair<double,double> thispair = *it;
       vout.push_back(thispair.first);
   }
   return vout;    
}

std::vector<double> CreateX2Vector(std::vector<std::pair<double,double>>& v){

   std::vector<double> vout;
   for (auto it = v.begin(); it!=v.end(); ++it){
       std::pair<double,double> thispair = *it;
       vout.push_back(thispair.second);
   }
   return vout;    
}

