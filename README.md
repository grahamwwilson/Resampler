# Resampler
Basic idea. Explore goodness-of-fit of bi-variate 
data-set to MC model using permutation test. Inspired 
initially by arXiv:1006.3019[hep-ex] and references therein, 
especially the Aslan and Zech ``statistical energy'' method.

## Current application focus
Current development is focused on goodness-of-fit estimates 
comparing beam-beam simulations of the ILC luminosity distribution 
in (x1, x2) space after beam energy spread and beamstrahlung 
using Guinea-PIG (the "data") with a parametrized model that has been fitted 
using reweighting to the same data, and from which toy "MC" data-sets are generated.
The reweighting fits were done with 100*100 approximately 
equiprobable cells in (x1,x2) space using 10M MC events to 171,349 GP events 
resulting in a chi-squared of 10199 for 9989 dof for the 10 parameter "CoPa" model. 
While this fit is pretty good, it neglects a lot of information, and so may 
not be as powerful a test of whether the model agrees with the data as may be 
desirable. Modifying the fit to use more cells is another option but this 
would likely need a switch to a full unbinned or Poisson likelihood approach 
including incorporation of some convolution integrals rather than 
the reweighting approach. 

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
