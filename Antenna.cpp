#include "Antenna.hpp"

const double PI = 3.14159265358979;

Antenna::Antenna (string antName, double freq, double antW, double antH, double antEffic, double pdstl, int tapType, double offAng) {						
	antennaName = antName;
	frequency = freq;
	width = antW;
	height = antH;
	efficiency = antEffic;
	pedestal = pdstl;
	taperType = tapType;
	offAxisAngle = offAng;
}

string Antenna::getAntennaName() { return antennaName; }
double Antenna::getFrequency() { return frequency; }
double Antenna::getEfficiency() { return efficiency; }
double Antenna::getOffAxisAngle() { return offAxisAngle; }
double Antenna::getPedestal() { return pedestal; }
int Antenna::getTaperType() { return taperType; }
double Antenna::getOffAxisGain() { return offAxisGain; }

string Antenna::getDimensions() {
	string dimensions = "";
	dimensions += "W = " + std::to_string(width) + " m";
	dimensions += " and H = " + std::to_string(height) + " m";
	return dimensions;
}

double Antenna::getPeakGain() {
	peakGain = 10 * log10(efficiency) + 10 * log10(4 * PI * pow(10 * frequency/2.9979248, 2) * PI * width/2 * height/2);
	return peakGain;
}

double Antenna::getRollOff() {
	double bessel_angle;
	if (width == height) {
		bessel_angle = 2 * PI * frequency * 1000/299.79248 * width/2 * sin(offAxisAngle * PI/180);
	} else {
		bessel_angle = 2 * PI * frequency * 1000/299.79248 * sqrt(width/2 * height/2) * sin(offAxisAngle * PI/180); // this will need to be adjusted for ASI calcs
	}
	double rolloff_numerator = (pow(10, pedestal/20) * (2 * integerBessel(bessel_angle,1)/bessel_angle) + (1 - pow(10,pedestal/20))/(taperType + 1) *
									pow(2, taperType + 1) * factorial(taperType + 1) * integerBessel(bessel_angle, taperType + 1)/pow(bessel_angle, taperType + 1));			
	double rolloff_denominator = (pow(10,pedestal/20) + (1 - pow(10,pedestal/20))/(taperType + 1));			
	rollOff = 10 * log10 (pow(rolloff_numerator / rolloff_denominator, 2));
	return rollOff;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating Bessel functions. By changing the the number of recursions, "even_check", you can increase the number 	/////
/////		of beam widths or distance for which this function is accurate.  The function provides results that are within 1% of the MS Excel 		/////
/////		Bessel function for across approximately:																								/////
/////			+/-4 beamwidths for bessel_count = 4																								/////
/////			+/-10 beamwidths for bessel_count = 40																								/////
/////			+/-25 beamwidths for bessel_count = 100																								/////
/////	The general (approximate) rule of thumb is that bessel_count/4 = number of beam widths.														/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double Antenna::integerBessel(double bessel_input, int bessel_order)  {
	// Labels: e100,e200
	int bessel_count,even_check;
	double bessel_temp,xn;
	double Y[5];
	double bessel_output;
	// Test for range
//			bessel_input = abs(bessel_input);
	Y[0] = 1.0;
	Y[1] = 0.0;
	bessel_temp = 0.0; 
	even_check = 20;
	
	// Update results
	e100: for (bessel_count = 4; bessel_count > 0; bessel_count--) {
		Y[bessel_count] = Y[bessel_count - 1];
	}
	  
	// Apply recursion relation
	xn = (double) even_check;
	Y[0] = 2.0 * xn * Y[1]/bessel_input - Y[2];
	even_check--;
	if (even_check == 0) goto e200;
	if (even_check % 2) goto e100; // if n is odd
	bessel_temp += 2 * Y[0];
	goto e100;
	e200: bessel_temp += Y[0];
	
	// Scale the results			  
	for (bessel_count = 0; bessel_count < 5; bessel_count++) {
	  Y[bessel_count] = Y[bessel_count]/bessel_temp;
	}
	  
	  //Output the desired Bessel Function
	switch (bessel_order) {
		case 0:
			bessel_output = Y[0];
			break;
		case 1:
			bessel_output = Y[1];
			break;
		case 2:
			bessel_output = Y[2];
			break;
		case 3:
			bessel_output = Y[3];
			break;
		case 4:
			bessel_output = Y[4];
			break;
	}
	return bessel_output;
}	
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the factorial of a number using recursion															 	/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
int Antenna::factorial(int fact_input) {

	if(fact_input > 1)
		return fact_input  * factorial(fact_input  - 1);
	else
		return 1;
}

string Antenna::toString() {
	return "Temporary Output String";
}