#include "LinearRegression.h"

// Constructor
LinearRegression::LinearRegression(TimeSeries* Y, TimeSeries* X)
{
	double sumXY = 0.0;
	double sumX = 0.0;
	double sumY = 0.0;
	double sumX2 = 0.0;
	double sumY2 = 0.0;

	this->Y = Y;
	this->X = X;

	// For every value in the sample of the two time series
	for(TimeSeries::iterator itY = this->Y->in_sample_begin(), itX = this->X->in_sample_begin(); itY != this->Y->in_sample_end() && itX != this->X->in_sample_end(); ++itY, ++itX)
	{
		sumXY += (*itX).second->get_value() * (*itY).second->get_value();
		sumX += (*itX).second->get_value();
		sumY += (*itY).second->get_value();
		sumX2 += (*itX).second->get_value() * (*itX).second->get_value();
		sumY2 += (*itY).second->get_value() * (*itY).second->get_value();
	}

	// Correlation between the two time series
	this->rho = (this->Y->sample_size() * sumXY - sumX * sumY) / (sqrt((this->Y->sample_size() * sumX2 - sumX * sumX) * (this->Y->sample_size() * sumY2 - sumY * sumY)));

	// Intercept of the linear regression
	this->beta = (this->Y->sample_size() * sumXY - sumX * sumY) / (this->Y->sample_size() * sumX2 - sumX * sumX);

	// Slope of the linear regression
	this->alpha = (sumY - this->beta * sumX) / this->Y->sample_size();

	return;
}

// Destructor
LinearRegression::~LinearRegression()
{
	return;
}

// Re-estimate the linear regression
void LinearRegression::reset()
{
	double sumXY = 0.0;
	double sumX = 0.0;
	double sumY = 0.0;
	double sumX2 = 0.0;
	double sumY2 = 0.0;

	// For every value in the sample of the two time series
	for(TimeSeries::iterator itY = this->Y->in_sample_begin(), itX = this->X->in_sample_begin(); itY != this->Y->in_sample_end() && itX != this->X->in_sample_end(); ++itY, ++itX)
	{
		sumXY += (*itX).second->get_value() * (*itY).second->get_value();
		sumX += (*itX).second->get_value();
		sumY += (*itY).second->get_value();
		sumX2 += (*itX).second->get_value() * (*itX).second->get_value();
		sumY2 += (*itY).second->get_value() * (*itY).second->get_value();
	}

	// Correlation between the two time series
	this->rho = (this->Y->sample_size() * sumXY - sumX * sumY) / (sqrt((this->Y->sample_size() * sumX2 - sumX * sumX) * (this->Y->sample_size() * sumY2 - sumY * sumY)));

	// Intercept of the linear regression
	this->beta = (this->Y->sample_size() * sumXY - sumX * sumY) / (this->Y->sample_size() * sumX2 - sumX * sumX);

	// Slope of the linear regression
	this->alpha = (sumY - this->beta * sumX) / this->Y->sample_size();

	return;
}

// Calculate the series of residuals
TimeSeries* LinearRegression::residuals(std::string name)
{
	TimeSeries* R;
	double value = 0.0;

	R = new TimeSeries(name);

	// For every value in the two time series
	for(TimeSeries::iterator itY = this->Y->series_begin(), itX = this->X->series_begin(); itY != this->Y->series_end() && itX != this->X->series_end(); ++itY, ++itX)
	{
		// R(t) = Y(t) - alpha - beta * X(t)
		value = (*itY).second->get_value() - this->get_alpha() - this->get_beta() * (*itX).second->get_value();
		R->insert((*itY).second->get_date(), value);
	}

	R->set_sample_date(this->Y->get_sample_date());

	return R;
}

// Getter of the correlation
double LinearRegression::get_rho()
{
	return this->rho;
}

// Getter of the intercept
double LinearRegression::get_alpha()
{
	return this->alpha;
}

// Getter of the slope
double LinearRegression::get_beta()
{
	return this->beta;
}
