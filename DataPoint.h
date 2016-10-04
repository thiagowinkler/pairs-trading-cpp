#ifndef DATAPOINT_H_
#define DATAPOINT_H_

#include <string>

class DataPoint
{
private:
	std::string date; // data point date
	double value;     // data point value

public:
	DataPoint(std::string, double); // constructor
	virtual ~DataPoint();           // destructor

	// Modifiers
	void set_date(std::string);     // setter of the data point date
	void set_value(double);         // setter of the data point value

	// Observers
	std::string get_date();         // getter of the data point date
	double get_value();             // getter of the data point value
};

#endif /* DATAPOINT_H_ */
