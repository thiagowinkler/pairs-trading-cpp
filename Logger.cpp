#include "Logger.h"

Logger* Logger::instance = nullptr; // initialization of the static pointer

// Getter of the instance of the class
Logger* Logger::get_instance()
{
	if (!instance) // only one instance of the class can be created
	{
		instance = new Logger();
	}

	return instance;
}

// Open a new log file
bool Logger::open_log_file(std::string file_path)
{
	this->log.open(file_path);

	return this->log.is_open();
}

// Write a line to the log file
void Logger::write_log_line(std::string line, bool verbose)
{
	this->log << line << std::endl;

	// If verbose, also print line on console
	if(verbose)
		std::cout << line << std::endl;
}

// Close the current log file
bool Logger::close_log_file()
{
	this->log.close();

	return true;
}
