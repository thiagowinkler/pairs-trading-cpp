#ifndef SPREADTIMESERIES_H_
#define SPREADTIMESERIES_H_

#include <cmath>
#include "DecoratedTimeSeries.h"

// Decorator pattern
class SpreadTimeSeries : public DecoratedTimeSeries
{
private:
	double series_mean; // time series mean
	double series_sd;   // time series standard deviation
	double sample_mean; // in-sample period mean
	double sample_sd;   // in-sample period standard deviation

public:
	SpreadTimeSeries(TS* inner);               // constructor
	virtual ~SpreadTimeSeries();               // destructor

	// Modifiers
	virtual bool insert(std::string, double);  // insert new elements in the time series
	virtual void set_sample_date(std::string); // setter for the in-sample period end date
	virtual void reset_sample_date();          // remove the in/out-of-sample boundary

	// (Lazy) Observers
	double get_series_mean();                  // getter for the mean of the time series
	double get_series_sd();                    // getter for the standard deviation of the time series
	double get_sample_mean();                  // getter for the mean of the in-sample series
	double get_sample_sd();                    // getter for the standard deviation of the in-sample series
};

#endif /* SPREADTIMESERIES_H_ */
