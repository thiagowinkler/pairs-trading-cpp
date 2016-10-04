#include <cmath>
#include <iostream>
#include <vector>

#include "DataPoint.h"
#include "LinearRegression.h"
#include "Logger.h"
#include "SpreadTimeSeries.h"
#include "TimeSeries.h"

using namespace std;

// Struct to hold the number of standard deviations for each band
struct bands
{
	double mean;
	double trade;
	double stop;
};

// Pairs Trading Strategy
//
//        Y : dependent variable time series
//        X : independent variable time series
//        B : number of standard deviations to be used in each band
//  verbose : true for printing/logging strategy results (default = true)
// backtest : false for out-of-sample execution, true for in-sample (default = false)
pair<double, int> pairs_trading(TimeSeries* Y, TimeSeries* X, struct bands B, bool verbose = true, bool backtest = false)
{
	TimeSeries::iterator itY;           // iterators for the Y time series
	TimeSeries::iterator endY;
	TimeSeries::reverse_iterator ritY;

	TimeSeries::iterator itX;           // iterators for the X time series
	TimeSeries::iterator endX;
	TimeSeries::reverse_iterator ritX;

	LinearRegression* LR;               // linear regression object pointer

	// Spread between Y and alpha + beta * X
	SpreadTimeSeries* S;
	SpreadTimeSeries::iterator itS;     // iterators for the Spread time series
	SpreadTimeSeries::iterator endS;
	SpreadTimeSeries::iterator itSprev;

	double upper_stop_signal = 0.0;     // band barrier variables
	double upper_trade_signal = 0.0;
	double upper_mean_signal = 0.0;
	double lower_mean_signal = 0.0;
	double lower_trade_signal = 0.0;
	double lower_stop_signal = 0.0;

	int current_positionY = 0;          // variable to hold the current position in Y
                                        // (2 = stop sell, 1 = buy, 0 = none, -1 = sell, -2 = stop buy)
	vector<double> buy_priceY;          // vectors to hold prices of positions
	vector<double> sell_priceY;
	vector<double> buy_priceX;
	vector<double> sell_priceX;

	double total_returnY = 0.0;         // variables to hold the total return of each instrument
	double total_returnX = 0.0;

	pair<double, int> results = pair<double, int>(0.0, 0); // output pair (total return, round trips)

	LR = new LinearRegression(Y, X);    // linear regression estimation

	S = new SpreadTimeSeries(LR->residuals("Spread"));     // spread time series evaluation

	// Calculate band barriers
	upper_stop_signal = S->get_sample_mean() + B.stop * S->get_sample_sd();
	upper_trade_signal = S->get_sample_mean() + B.trade * S->get_sample_sd();
	upper_mean_signal = S->get_sample_mean() + B.mean * S->get_sample_sd();
	lower_mean_signal = S->get_sample_mean() - B.mean * S->get_sample_sd();
	lower_trade_signal = S->get_sample_mean() - B.trade * S->get_sample_sd();
	lower_stop_signal = S->get_sample_mean() - B.stop * S->get_sample_sd();

	if(verbose)
	{
		// (long double) is a fix for Visual Studio versions prior to 2012
		Logger::get_instance()->write_log_line("The linear regression is " + Y->get_name() + " = " + to_string((long double) LR->get_alpha()) + " + " + to_string((long double) LR->get_beta()) + X->get_name() + "\n", verbose);

		// (long double) is a fix for Visual Studio versions prior to 2012
		Logger::get_instance()->write_log_line("The standard deviation of the " + S->get_name() + " time series is " + to_string((long double) S->get_sample_sd()) + ", so:", verbose);
		Logger::get_instance()->write_log_line("- upper stop signal: " + to_string((long double) upper_stop_signal) + ";", verbose);
		Logger::get_instance()->write_log_line("- upper trade signal: " + to_string((long double) upper_trade_signal) + ";", verbose);
		Logger::get_instance()->write_log_line("- upper mean signal: " + to_string((long double) upper_mean_signal) + ";", verbose);
		Logger::get_instance()->write_log_line("- lower mean signal: " + to_string((long double) lower_mean_signal) + ";", verbose);
		Logger::get_instance()->write_log_line("- lower trade signal: " + to_string((long double) lower_trade_signal) + ";", verbose);
		Logger::get_instance()->write_log_line("- lower stop signal: " + to_string((long double) lower_stop_signal) + ".\n", verbose);

		Logger::get_instance()->write_log_line("Here are the trading rules:", verbose);
		Logger::get_instance()->write_log_line("- if " + S->get_name() + " hits the upper trading signal: sell 1 unit of " + Y->get_name() + " and buy beta units of " + X->get_name() + ";", verbose);
		Logger::get_instance()->write_log_line("- if " + S->get_name() + " hits the lower trading signal: buy 1 unit of " + Y->get_name() + " and sell beta units of " + X->get_name() + ";", verbose);
		Logger::get_instance()->write_log_line("- if " + S->get_name() + " hits any of the stop signals: close current positions;", verbose);
		Logger::get_instance()->write_log_line("- when the value of " + S->get_name() + " returns to its mean area: close current positions (if any), and begin again.\n", verbose);
	}

	// If performing a backtest, set iterators to in-sample period
	if(backtest)
	{
		itY = Y->in_sample_begin();
		endY = Y->in_sample_end();
		ritY = Y->in_sample_rbegin();

		itX = X->in_sample_begin();
		endX = X->in_sample_end();
		ritX = X->in_sample_rbegin();

		itS = S->in_sample_begin();
		endS = S->in_sample_end();

		if(verbose)
		{
			Logger::get_instance()->write_log_line("Here are the backtesting results for the period ending in " + S->get_sample_date() + ":", verbose);
		}
	}
	// Else, set iterators to out-of-sample period
	else
	{
		itY = Y->out_sample_begin();
		endY = Y->out_sample_end();
		ritY = Y->out_sample_rbegin();

		itX = X->out_sample_begin();
		endX = X->out_sample_end();
		ritX = X->out_sample_rbegin();

		itS = S->out_sample_begin();
		endS = S->out_sample_end();

		if(verbose)
		{
			Logger::get_instance()->write_log_line("Here are the results for the period starting the day after " + S->get_sample_date() + ":", verbose);
		}
	}

	// Strategy Evaluation ---------------------------------------------------------

	itSprev = itS;
	++itS;
	++itY;
	++itX;

	for(; itS != endS && itY != endY && itX != endX; ++itSprev, ++itS, ++itY, ++itX)
	{
		// Test if crossed upper stop signal: if true, close all current positions
		if ((*itS).second->get_value() > upper_stop_signal && (*itSprev).second->get_value() < upper_stop_signal && current_positionY == -1)
		{
			current_positionY = -2;
			buy_priceY.push_back(log((*itY).second->get_value()));
			sell_priceX.push_back(log(LR->get_beta() * (*itX).second->get_value()));
		}
		// Test if crossed upper trade signal: if true, sell 1 unit of Y, and buy beta units of X
		else if ((*itS).second->get_value() > upper_trade_signal && (*itSprev).second->get_value() < upper_trade_signal && current_positionY == 0)
		{
			current_positionY = -1;
			sell_priceY.push_back(log((*itY).second->get_value()));
			buy_priceX.push_back(log(LR->get_beta() * (*itX).second->get_value()));
		}
		// Test if crossed upper mean signal: if true, close all current positions
		else if ((*itS).second->get_value() < upper_mean_signal && (*itSprev).second->get_value() > upper_mean_signal)
		{
			if(current_positionY == -1)
			{
				buy_priceY.push_back(log((*itY).second->get_value()));
				sell_priceX.push_back(log(LR->get_beta() * (*itX).second->get_value()));
			}
			current_positionY = 0;
		}
		// Test if crossed lower mean signal: if true, close all current positions
		else if ((*itS).second->get_value() > lower_mean_signal && (*itSprev).second->get_value() < lower_mean_signal)
		{
			if(current_positionY == 1)
			{
				sell_priceY.push_back(log((*itY).second->get_value()));
				buy_priceX.push_back(log(LR->get_beta() * (*itX).second->get_value()));
			}
			current_positionY = 0;
		}
		// Test if crossed lower trade signal: if true, buy 1 unit of Y, and sell beta units of X
		else if ((*itS).second->get_value() < lower_trade_signal && (*itSprev).second->get_value() > lower_trade_signal && current_positionY == 0)
		{
			current_positionY = 1;
			buy_priceY.push_back(log((*itY).second->get_value()));
			sell_priceX.push_back(log(LR->get_beta() * (*itX).second->get_value()));
		}
		// Test if crossed lower stop signal: if true, close all current positions
		else if ((*itS).second->get_value() < lower_stop_signal && (*itSprev).second->get_value() > lower_stop_signal && current_positionY == 1)
		{
			current_positionY = 2;
			sell_priceY.push_back(log((*itY).second->get_value()));
			buy_priceX.push_back(log(LR->get_beta() * (*itX).second->get_value()));
		}
	}

	// If there are still open positions by the end of the strategy period, close them
	if(current_positionY == 1)
	{
		sell_priceY.push_back(log((*ritY).second->get_value()));
		buy_priceX.push_back(log(LR->get_beta() * (*ritX).second->get_value()));
	}
	else if(current_positionY == -1)
	{
		buy_priceY.push_back(log((*ritY).second->get_value()));
		sell_priceX.push_back(log(LR->get_beta() * (*ritX).second->get_value()));
	}

	// Calculate total return for each instrument
	for(size_t i = 0; i < buy_priceY.size(); ++i)
	{
		total_returnY += (sell_priceY[i] - buy_priceY[i]);
		total_returnX += (sell_priceX[i] - buy_priceX[i]);
	}

	// Set up output pair
	results.first = total_returnY + total_returnX;
	results.second = buy_priceY.size();

	// (long double) and (long long) are fixes for Visual Studio versions prior to 2012
	Logger::get_instance()->write_log_line("- total return: " + to_string((long double) results.first) + ";", verbose);
	Logger::get_instance()->write_log_line("- round trips: " + to_string((long long) results.second) + ".", verbose);

	// Delete used pointers
	delete LR;
	delete S;

	return results;
}

int main()
{
	TimeSeries* Y;  // time series object pointer for the dependent variable
	TimeSeries* X;  // time series object pointer for the dependent variable
	struct bands B; // struct to hold the number of standard deviations for each band

	Logger::get_instance()->open_log_file("log/log.txt");       // set up log file

	Y = new TimeSeries("GE", "input/General Electric.csv");     // set up time series Y
	Y->set_sample_date("2014-10-31");
	X = new TimeSeries("UTX", "input/United Technologies.csv"); // set up time series X
	X->set_sample_date("2014-10-31");

	memset(&B, 0, sizeof(struct bands));                        // set up bands
	B.mean = 0.5;
	B.trade = 1.0;
	B.stop = 2.5;

//	Y = new TimeSeries("JPM", "input/JPMorgan Chase.csv");      // set up time series Y
//	Y->set_sample_date("2014-10-31");
//	X = new TimeSeries("GS", "input/Goldman Sachs.csv");        // set up time series X
//	X->set_sample_date("2014-10-31");

//	memset(&B, 0, sizeof(struct bands));                        // set up bands
//	B.mean = 0.5;
//	B.trade = 1.0;
//	B.stop = 4;

	pairs_trading(Y, X, B);                                     // execute trading strategy

//	pair<double, int> results;
//	double max_return = 0.0;
//	double mean_band = 0.0;
//	double trade_band = 0.0;
//	double stop_band = 0.0;
//
//	memset(&B, 0, sizeof(struct bands));
//
//	for(double m = 0.5; m <= 5.0; m += 0.5)
//	{
//		for(double t = m + 0.5; t <= 5.0; t += 0.5)
//		{
//			for(double s = t + 0.5; s <= 5.0; s += 0.5)
//			{
//				B.mean = m;
//				B.trade = t;
//				B.stop = s;
//
//				results = pairs_trading(Y, X, B, false, true);
//
//				if(max_return < results.first)
//				{
//					max_return = results.first;
//					mean_band = m;
//					trade_band = t;
//					stop_band = s;
//				}
//			}
//		}
//	}
//
//	cout << max_return << " " << mean_band << " " << trade_band << " " << stop_band << endl;

	// Delete used pointers
	delete Y;
	delete X;

	return 0;
}
