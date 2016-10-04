#ifndef LINEARREGRESSION_H_
#define LINEARREGRESSION_H_

#include <cmath>
#include <string>
#include "TimeSeries.h"

// Mediator pattern
class LinearRegression
{
private:
	TimeSeries* Y; // dependent variable of the linear regression
	TimeSeries* X; // independent variable of the linear regression
	double rho;    // correlation between the two time series
	double alpha;  // intercept of the linear regression
	double beta;   // slope of the linear regression

public:
	LinearRegression(TimeSeries*, TimeSeries*); // constructor
	virtual ~LinearRegression();                // destructor

	void reset();                               // re-estimate the linear regression
	TimeSeries* residuals(std::string);         // calculate the series of residuals

	// Observers
	double get_rho();                           // getter of the correlation
	double get_alpha();                         // getter of the intercept
	double get_beta();                          // getter of the slope
};

#endif /* LINEARREGRESSION_H_ */
