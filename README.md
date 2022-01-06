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
Log of old developments is in OldREADME.md

## example11.cpp
Latest version. Includes 14 different estimator choices.
See run17.sh for an example of how to run.

Latest comparisons are in Comparison.txt.
