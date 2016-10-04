#include "SpreadTimeSeries.h"

// Constructor
SpreadTimeSeries::SpreadTimeSeries(TS* inner) : DecoratedTimeSeries(inner)
{
	this->series_mean = 0.0;
	this->series_sd = 0.0;
	this->sample_mean = 0.0;
	this->sample_sd = 0.0;

	return;
}

// Destructor
SpreadTimeSeries::~SpreadTimeSeries()
{
	return;
}

// Insert new elements in the time series
bool SpreadTimeSeries::insert(std::string date, double value)
{
	bool inserted = false;
	struct tm tm = {0};
	int year = 0;
	int month = 0;
	int day = 0;

	inserted = DecoratedTimeSeries::insert(date, value);

	// If the new data point was successfully inserted
	if(inserted)
	{
		this->series_mean = 0.0;                 // reset the time series mean
		this->series_sd = 0.0;                   // reset the time series standard deviation

		sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day); // convert date to time stamp
		tm.tm_year = year - 1900;
		tm.tm_mon = month - 1;
		tm.tm_mday = day;

		// If the new data point was in the in-sample period
		if(mktime(&tm) <= this->get_sample_timestamp())
		{
			this->sample_mean = 0.0;            // reset the in-sample period mean
			this->sample_sd = 0.0;              // reset the in-sample period standard deviation
		}
	}

	return inserted;
}

// Setter for the in-sample period end date
void SpreadTimeSeries::set_sample_date(std::string sample_date)
{
	DecoratedTimeSeries::set_sample_date(sample_date);

	this->sample_mean = 0.0; // reset the in-sample period mean
	this->sample_sd = 0.0;   // reset the in-sample period standard deviation

	return;
}

// remove the in/out-of-sample boundary
void SpreadTimeSeries::reset_sample_date()
{
	DecoratedTimeSeries::reset_sample_date();

	this->sample_mean = this->get_series_mean(); // in-sample period mean = time series mean
	this->sample_sd = this->get_series_sd();     // in-sample period standard deviation = time series standard deviation

	return;
}

// Getter for the mean of the time series
double SpreadTimeSeries::get_series_mean()
{
	double sum = 0.0;

	// Lazy Initialization pattern
	if(this->series_mean == 0.0)
	{
		for(TimeSeries::iterator it = this->series_begin(); it != this->series_end(); ++it)
		{
			sum += (*it).second->get_value();
		}

		this->series_mean = sum / this->series_size();

		// If there is no in/out-of-sample boundary
		if(this->get_sample_timestamp() == 0)
		{
			this->sample_mean = this->series_mean;
		}
	}

	return this->series_mean;
}

// Getter for the standard deviation of the time series
double SpreadTimeSeries::get_series_sd()
{
	double sum_squares = 0.0;

	// Lazy Initialization pattern
	if(this->series_sd == 0.0)
	{
		for(TimeSeries::iterator it = this->series_begin(); it != this->series_end(); ++it)
		{
			sum_squares += (*it).second->get_value() * (*it).second->get_value();
		}

		this->series_sd = std::sqrt(sum_squares / this->series_size() - this->get_series_mean() * this->get_series_mean());

		// If there is no in/out-of-sample boundary
		if(this->get_sample_timestamp() == 0)
		{
			this->sample_sd = this->series_sd;
		}
	}

	return this->series_sd;
}

// Getter for the mean of the in-sample series
double SpreadTimeSeries::get_sample_mean()
{
	double sum = 0.0;

	// Lazy Initialization pattern
	if(this->sample_mean == 0.0)
	{
		for(TimeSeries::iterator it = this->in_sample_begin(); it != this->in_sample_end(); ++it)
		{
			sum += (*it).second->get_value();
		}

		this->sample_mean = sum / this->sample_size();
	}

	return this->sample_mean;
}

// Getter for the standard deviation of the in-sample series
double SpreadTimeSeries::get_sample_sd()
{
	double sum_squares = 0.0;

	// Lazy Initialization pattern
	if(this->sample_sd == 0.0)
	{
		for(TimeSeries::iterator it = this->in_sample_begin(); it != this->in_sample_end(); ++it)
		{
			sum_squares += (*it).second->get_value() * (*it).second->get_value();
		}

		this->sample_sd = std::sqrt(sum_squares / this->sample_size() - this->get_sample_mean() * this->get_sample_mean());
	}

	return this->sample_sd;
}
