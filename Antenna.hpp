#ifndef ANTENNA_H
#define ANTENNA_H

#include <cmath>
#include "Defines.hpp"

class Antenna {

private:
	string antennaName;
	double frequency;
	double width;
	double height;
	double efficiency;
	double pedestal;
	int taperType;
	double offAxisAngle;
	double offAxisGain;
	double peakGain;
	double rollOff;
	struct rollOffMatrix {
		double angle;
		double antRollOff;
	};
	vector <rollOffMatrix> relativeGainVsAngle;
	
	double integerBessel(double bessel_input, int bessel_order);
	int factorial(int fact_input);
	
	
public: 
	Antenna (string antName, double freq, double antW, double antH, double antEffic, double pdstl, int tapType, double offAng);
	
	Antenna() {};
	string getAntennaName();
	double getFrequency();
	string getDimensions();
	double getEfficiency();
	double getOffAxisAngle();
	double getPedestal();
	int getTaperType();
	double getOffAxisGain();
	double getRollOff();
	double getPeakGain();
	void getAntennaCut();
	string toString();
};

#endif
	
