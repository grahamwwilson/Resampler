const int NESTIMATORS = 6;
enum Estimators{MeanECM, MeanEdiff, Energy, Energy2, KSECM, KSEdiff};
const std::string Names[NESTIMATORS] =
{ "Mean ECM offset (%)", "Mean scaled Ediff (%)", 
  "Energy Statistic", "2nd Energy Statistic", 
  "New ECM KS-test q-value", "New Ediff KS-test q-value"};
