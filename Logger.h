#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <iostream>
#include <string>

// Singleton pattern
class Logger
{
private:
	static Logger* instance; // single instance of the class

	std::ofstream log;       // log file

	Logger() {};                                            // private constructor
	Logger(Logger const&) {};                               // private copy constructor
	Logger& operator=(Logger const&) { return *instance; }; // private assignment operator

public:
	static Logger* get_instance();                          // getter of the instance of the class
	bool open_log_file(std::string);                        // open a new log file
	void write_log_line(std::string, bool);                 // write a line to the log file
	bool close_log_file();                                  // close the current log file
};

#endif /* LOGGER_H_ */
