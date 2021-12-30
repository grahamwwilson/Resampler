# Resampler
Basic idea. Explore goodness-of-fit of bi-variate 
data-set to MC model using permutation test. Inspired 
initially by arXiv:1006.3019[hep-ex] and references therein, 
especially the Aslan and Zech ``statistical energy'' method.
Code also has some basic histogramming and other standard tests like KS test.
The same method can be used for two-sample data to data tests.

## Current application focus
Current development is focused on goodness-of-fit estimates 
comparing beam-beam simulations of the ILC luminosity distribution 
in (x1, x2) space after beam energy spread and beamstrahlung 
using Guinea-PIG (the "data") with a parametrized model that has been fitted 
using reweighting to the same data, and from which toy "MC" data-sets are generated.
The reweighting fits were done with 100*100 approximately 
equiprobable cells in (x1,x2) space using 10M MC events to 171,349 GP events (Run5).
This resulted in a chi-squared of 10199 for 9989 dof for the 10 parameter "CoPa" model. 
While this fit is pretty good, it neglects a lot of information, and so may 
not be as powerful a test of whether the model agrees with the data as may be 
desirable. Modifying the fit to use more cells is another option but this 
would likely need a switch from the reweighting approach to a full unbinned 
or Poisson likelihood including incorporation of some convolution integrals.
Current test metrics and more details are in Tests.txt

## Log
This is more a log of the developments. Each example is 
turned into a distinct version as they acquire more functionality.

## example.cpp
reads in a couple of files and makes plots.

## example2.cpp
First real implementation of a simple mean-based 
permutation test to test whether the "data" distribution is consistent with 
the "MC" distribution using a particular test statistic. Currently 
use mean ECM as statistic.

## example3.cpp
Alternatively use mean pz as statistic.

## example4.cpp
Choose between ECM offset and pz as statistic
Include KS tests for both of these 1-d distributions.

## example5.cpp and run2.sh
Integrate statistic choice for permutation test into the script.

## example6.cpp and run4.sh
Change dataset names to be more generic and do data to data comparisons here.

## example6.cpp and run5.sh
Now also include permutation test for 1-d ECM KS test - more as a verification/validation 
of the permutation method in the context of a statistic with well known properties.

## example7.cpp and run7.sh
First implementation of energy statistic. Rather slow as expected 
given N^2 complexity on even the smallest problem size currently considered. 
About 16 minutes per permutation on laptop with Run5/Run7 comparison 
with about about 160k events in each sample.
Room for optimization/parallelization/batchification.
