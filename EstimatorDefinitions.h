const int NESTIMATORS = 14;
enum Estimator{MeanECM, MeanEdiff, MeanX1, MeanX2, 
               Energy, Energy2, 
               KSECM, KSEdiff, KSX1, KSX2, 
               ChiSquaredX1, ChiSquaredX2, ChiSquaredX1X2, ChiSquaredX1PX2};
const std::string EstimatorNames[NESTIMATORS] =
{ "Mean ECM offset (%)", "Mean scaled Ediff (%)", "Mean X1 (%)", "Mean X2 (%)",
  "Energy Statistic", "2nd Energy Statistic", 
  "New ECM KS-test q-value", "New Ediff KS-test q-value",
  "New X1 KS-test q-value", "New X2 KS-test q-value",
  "ChiSquaredX1","ChiSquaredX2","ChiSquaredX1X2","ChiSquaredX1+X2"};
  
// function to convert an input string to an Estimator
// See https://www.cplusplus.com/forum/beginner/174993/
//
// This is a bit nasty - but at least keeps all this in one place
//  

Estimator convert(const std::string& str)
{
   if(str == "MeanECM"){
      return MeanECM;
   }
   else if(str == "MeanEdiff"){
      return MeanEdiff;
   }
   else if(str == "MeanX1"){
      return MeanX1;
   }
   else if(str == "MeanX2"){
      return MeanX2;
   }   
   else if(str == "Energy"){
      return Energy;
   }
   else if(str == "Energy2"){
      return Energy2;
   }
   else if(str == "KSECM"){
      return KSECM;
   }
   else if(str == "KSEdiff"){
      return KSEdiff;
   }
   else if(str == "KSX1"){
      return KSX1;
   }
   else if(str == "KSX2"){
      return KSX2;
   }   
   else if(str == "ChiSquaredX1"){
      return ChiSquaredX1;
   }
   else if(str == "ChiSquaredX2"){
      return ChiSquaredX2;
   }
   else if(str == "ChiSquaredX1X2"){
      return ChiSquaredX1X2;
   }
   else if(str == "ChiSquaredX1+X2"){
      return ChiSquaredX1PX2;
   }   
   else{
   // Compiler prefers some return value
     return ChiSquaredX1;
   }
}
