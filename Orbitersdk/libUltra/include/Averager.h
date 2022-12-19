#ifndef AVERAGER_H_6FE4ACDE_09A1_4883_AD34_D715EF992F16
#define AVERAGER_H_6FE4ACDE_09A1_4883_AD34_D715EF992F16
#pragma once

#include <queue>

class Averager
{
	std::queue<double> previousValues;
	std::queue<double> previousTimestamps;
	double sum;
	double timeLimit;
	//unsigned int sizeLimit;
public:
	/**
	 * \param sampleSize (maxmimum) number of samples to average
	 */
	//Averager(unsigned int sampleSize);
	/**
	 * \param minTime minimum time (seconds) to average over
	 */
	Averager(double minTime);
	~Averager();
	
	//void NewValue(double val);
	void NewValue(double val, double timestamp);
	double GetAvgValue() const;
};

#endif // AVERAGER_H_6FE4ACDE_09A1_4883_AD34_D715EF992F16
