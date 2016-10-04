# pairs-trading-cpp
Pairs Trading Strategy Implementation in C++

### Input
Two CSV files (one per stock) with the following header:

Ticker,GE US Equity  
DATE,PX_LAST

(replace "GE US Equity" with the desired ticker) followed by a list of rows
containing a date and a closing price each.

Paths to the above files, along with trading band values, must be set in the
**main** function body. The commented section in **main** may be used to find
a set of band values using brute force (not really elegant). A cointegration
test is recommended to find the ideal stock pair.

### Implementation Details
Pairs trading is a fairly simple (and wildly known) trading strategy, that
doesn't really work in "the real world", but that is nice to learn because of
the concepts you need to know to understand it. My implementation does not take
transaction costs into account, and that is one of the reasons you may get
positive returns while playing around with it.

The code for this project is also way more complicated than it could
potentially be. The idea was to force me to use some design patterns in C++,
that I didn't necessarily need, but wanted to learn how to implement.
