#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include "DataPoint.h"

// Decorator pattern (Abstract Class)
class TS
{
public:
	TS() {};                                         // constructor
	virtual ~TS() {};                                // destructor

	// Modifiers
	virtual bool insert(std::string, double) = 0;    // insert new elements in the time series
	virtual void set_sample_date(std::string) = 0;   // setter for the in-sample period end date
	virtual void reset_sample_date() = 0;            // remove the in/out-of-sample boundary

	// Observers
	virtual std::string get_name() = 0;              // getter for the name of the time series
	virtual std::string get_sample_date() = 0;       // getter for the in-sample period end date
	virtual time_t get_sample_timestamp() = 0;       // getter for the in-sample period end time stamp

	// Capacity
	virtual int series_size() = 0;                   // time series length
	virtual int sample_size() = 0;                   // in-sample series length

	// Delegation pattern (delegate the TS iterator logic to map's iterators)
	typedef std::map<time_t, DataPoint*>::iterator iterator;
	typedef std::map<time_t, DataPoint*>::const_iterator const_iterator;
	typedef std::map<time_t, DataPoint*>::reverse_iterator reverse_iterator;
	typedef std::map<time_t, DataPoint*>::const_reverse_iterator const_reverse_iterator;

	// Iterator pattern
	virtual iterator series_begin() = 0;              // iterator to the begin of the time series
	virtual iterator series_end() = 0;                // iterator to the end of the time series
	virtual reverse_iterator series_rbegin() = 0;     // reverse iterator to the end of the time series
	virtual reverse_iterator series_rend() = 0;       // reverse iterator to the begin of the time series
	virtual iterator in_sample_begin() = 0;           // iterator to the begin of the in-sample series
	virtual iterator in_sample_end() = 0;             // iterator to the end of the in-sample series
	virtual reverse_iterator in_sample_rbegin() = 0;  // reverse iterator to the end of the in-sample series
	virtual reverse_iterator in_sample_rend() = 0;    // reverse iterator to the begin of the in-sample series
	virtual iterator out_sample_begin() = 0;          // iterator to the begin of the out-of-sample series
	virtual iterator out_sample_end() = 0;            // iterator to the end of the out-of-sample series
	virtual reverse_iterator out_sample_rbegin() = 0; // reverse iterator to the end of the out-of-sample series
	virtual reverse_iterator out_sample_rend() = 0;   // reverse iterator to the begin of the out-of-sample series
};

// Decorator pattern (Concrete Class)
class TimeSeries : public TS
{
private:
	std::string name;                    // time series name
	std::map<time_t, DataPoint*> series; // time series data points
	std::string sample_date;             // in-sample period end date
	time_t sample_timestamp;             // in-sample period end time stamp

public:
	TimeSeries(std::string, std::string);         // two different constructors
	TimeSeries(std::string);
	virtual ~TimeSeries();                        // destructor

	// Modifiers
	virtual bool insert(std::string, double);     // insert new elements in the time series
	virtual void reset_sample_date();             // setter for the in-sample period end date
	virtual void set_sample_date(std::string);    // remove the in/out-of-sample boundary

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

#endif /* TIMESERIES_H_ */
