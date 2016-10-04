#ifndef DECORATEDTIMESERIES_H_
#define DECORATEDTIMESERIES_H_

#include "TimeSeries.h"

// Decorator pattern (Decorator Class)
class DecoratedTimeSeries : public TS
{
protected:
	TS* inner; // inner TS object pointer

public:
	DecoratedTimeSeries(TS*);                     // constructor
	virtual ~DecoratedTimeSeries();               // destructor

	// Modifiers
	virtual bool insert(std::string, double);     // insert new elements in the time series
	virtual void set_sample_date(std::string);    // setter for the in-sample period end date
	virtual void reset_sample_date();             // remove the in/out-of-sample boundary

	// Observers
	virtual std::string get_name();               // getter for the name of the time series
	virtual std::string get_sample_date();        // getter for the in-sample period end date
	virtual time_t get_sample_timestamp();        // getter for the in-sample period end time stamp

	// Capacity
	virtual int series_size();                    // time series length
	virtual int sample_size();                    // in-sample series length

	// Iterator pattern
	virtual iterator series_begin();              // iterator to the begin of the time series
	virtual iterator series_end();                // iterator to the end of the time series
	virtual reverse_iterator series_rbegin();     // reverse iterator to the end of the time series
	virtual reverse_iterator series_rend();       // reverse iterator to the begin of the time series
	virtual iterator in_sample_begin();           // iterator to the begin of the in-sample series
	virtual iterator in_sample_end();             // iterator to the end of the in-sample series
	virtual reverse_iterator in_sample_rbegin();  // reverse iterator to the end of the in-sample series
	virtual reverse_iterator in_sample_rend();    // reverse iterator to the begin of the in-sample series
	virtual iterator out_sample_begin();          // iterator to the begin of the out-of-sample series
	virtual iterator out_sample_end();            // iterator to the end of the out-of-sample series
	virtual reverse_iterator out_sample_rbegin(); // reverse iterator to the end of the out-of-sample series
	virtual reverse_iterator out_sample_rend();   // reverse iterator to the begin of the out-of-sample series
};

#endif /* DECORATEDTIMESERIES_H_ */
