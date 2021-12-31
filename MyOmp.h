#include <unordered_map>

#ifdef _OPENMP
   #include <omp.h>
   #define OMP_VERSION _OPENMP
#else
   #define omp_get_thread_num() 0
   #define omp_get_num_threads() 1
   #define OMP_VERSION 0
#endif

void checkomp();

void checkomp(){

// Check how we compiled this program and the OpenMP version
   std::cout << "OMP_VERSION " << OMP_VERSION << std::endl;
   std::unordered_map<unsigned int,std::string> map{ 
        {0,"None"},{200505,"2.5"},{200805,"3.0"},
        {201107,"3.1"},{201307,"4.0"},{201511,"4.5"} };
   if(OMP_VERSION!=0){
      printf("This program is compiled using OpenMP version %s\n", 
              map.at(OMP_VERSION).c_str());
   }
   else{
      std::cout << "This program is NOT compiled using OpenMP" << std::endl;
   }
}   
