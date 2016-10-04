#include "DecoratedTimeSeries.h"

// Constructor
DecoratedTimeSeries::DecoratedTimeSeries(TS* inner)
{
	this->inner = inner;

	return;
}

// Destructor
DecoratedTimeSeries::~DecoratedTimeSeries()
{
	return;
}

// Insert new elements in the time series
bool DecoratedTimeSeries::insert(std::string date, double value)
{
	return this->inner->insert(date, value);
}

// Setter for the in-sample period end date
void DecoratedTimeSeries::set_sample_date(std::string sample_date)
{
	this->inner->set_sample_date(sample_date);

	return;
}

// Remove the in/out-of-sample boundary
void DecoratedTimeSeries::reset_sample_date()
{
	this->inner->reset_sample_date();

	return;
}

// Getter for the name of the time series
std::string DecoratedTimeSeries::get_name()
{
	return this->inner->get_name();
}

// Getter for the in-sample period end date
std::string DecoratedTimeSeries::get_sample_date()
{
	return this->inner->get_sample_date();
}

// Getter for the in-sample period end time stamp
time_t DecoratedTimeSeries::get_sample_timestamp()
{
	return this->inner->get_sample_timestamp();
}

// Time series length
int DecoratedTimeSeries::series_size()
{
	return this->inner->series_size();
}

// In-sample series length
int DecoratedTimeSeries::sample_size()
{
	return this->inner->sample_size();
}

// Iterator to the begin of the time series
DecoratedTimeSeries::iterator DecoratedTimeSeries::series_begin()
{
	return this->inner->series_begin();
}

// Iterator to the end of the time series
DecoratedTimeSeries::iterator DecoratedTimeSeries::series_end()
{
	return this->inner->series_end();
}

// Reverse iterator to the end of the time series
DecoratedTimeSeries::reverse_iterator DecoratedTimeSeries::series_rbegin()
{
	return this->inner->series_rbegin();
}

// Reverse iterator to the begin of the time series
DecoratedTimeSeries::reverse_iterator DecoratedTimeSeries::series_rend()
{
	return this->inner->series_rend();
}

// Iterator to the begin of the in-sample series
DecoratedTimeSeries::iterator DecoratedTimeSeries::in_sample_begin()
{
	return this->inner->in_sample_begin();
}

// Iterator to the end of the in-sample series
DecoratedTimeSeries::iterator DecoratedTimeSeries::in_sample_end()
{
	return this->inner->in_sample_end();
}

// Reverse iterator to the end of the in-sample series
DecoratedTimeSeries::reverse_iterator DecoratedTimeSeries::in_sample_rbegin()
{
	return this->inner->in_sample_rbegin();
}

// Reverse iterator to the begin of the in-sample series
DecoratedTimeSeries::reverse_iterator DecoratedTimeSeries::in_sample_rend()
{
	return this->inner->in_sample_rend();
}

// Iterator to the begin of the out-of-sample series
DecoratedTimeSeries::iterator DecoratedTimeSeries::out_sample_begin()
{
	return this->inner->out_sample_begin();
}

// Iterator to the end of the out-of-sample series
DecoratedTimeSeries::iterator DecoratedTimeSeries::out_sample_end()
{
	return this->inner->out_sample_end();
}

// Reverse iterator to the end of the out-of-sample series
DecoratedTimeSeries::reverse_iterator DecoratedTimeSeries::out_sample_rbegin()
{
	return this->inner->out_sample_rbegin();
}

// Reverse iterator to the begin of the out-of-sample series
DecoratedTimeSeries::reverse_iterator DecoratedTimeSeries::out_sample_rend()
{
	return this->inner->out_sample_rend();
}
