#include "DataPoint.h"

// Constructor
DataPoint::DataPoint(std::string date, double value)
{
	this->date = date;
	this->value = value;

	return;
}

// Destructor
DataPoint::~DataPoint()
{
	return;
}

// Setter of the data point date
void DataPoint::set_date(std::string date)
{
	this->date = date;

	return;
}

// Setter of the data point value
void DataPoint::set_value(double value)
{
	this->value = value;

	return;
}

// Getter of the data point date
std::string DataPoint::get_date()
{
	return this->date;
}

// Getter of the data point value
double DataPoint::get_value()
{
	return this->value;
}
