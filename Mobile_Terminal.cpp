#include "Mobile_Terminal.hpp"

const double EARTH_RADIUS = 6378.137;
const long GEO_ALTITUDE = 35786;
const double PI = 3.14159265358979;
const double GEO_ORBIT_RADIUS = GEO_ALTITUDE + EARTH_RADIUS;

Mobile_Terminal::Mobile_Terminal (string termName, bool elTrk, double lat, double lon, double rxFreq, double txFreq, double antW, double antH, double antEffic, double pdstl, int tapType, 
								  double misptg, string mdm, double orbPosn, Neighbor_Satellite neighbor) {						
	terminalName = termName;
	elevationTracking = elTrk;
	latitude = lat;
	longitude = lon;
	rxFrequency = rxFreq;
	txFrequency = txFreq;
	width = antW;
	height = antH;
	efficiency = antEffic;
	pedestal = pdstl;
	taperType = tapType;
	mispointing = misptg;
	modem = mdm;
	orbitalPosition = orbPosn;
}

string Mobile_Terminal::getTerminalName() { return terminalName; }
double Mobile_Terminal::getLatitude() { return latitude; }
double Mobile_Terminal::getLongitude() { return longitude; }
double Mobile_Terminal::getRxFrequency() { return rxFrequency; }
double Mobile_Terminal::getTxFrequency() { return txFrequency; }
double Mobile_Terminal::getEfficiency() { return efficiency; }
double Mobile_Terminal::getPedestal() { return pedestal; }
double Mobile_Terminal::getMispointing() { return mispointing; }
int Mobile_Terminal::getTaperType() { return taperType; }

string Mobile_Terminal::getDimensions() {
	string dimensions = "";
	dimensions += "W = " + std::to_string(width) + " m";
	dimensions += " and H = " + std::to_string(height) + " m";
	return dimensions;
}

bool Mobile_Terminal::getElevationTrackingStatus() { return elevationTracking; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In these functions we are calculating the terminal azimuth and elevation toward the satellite in use.  0 deg Az is due north and Az 		/////
/////		increases as we go clockwise (i.e. 90 deg Az is due east from the terminal).  0 deg El is tangential to earth surface and 90 deg El is	/////
/////		"straight up."																																	/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double Mobile_Terminal::getTerminalElevation() {
	double delta_long;
	delta_long = orbitalPosition - longitude;
	terminalElevation = 180/PI * atan((cos(PI/180 * latitude) * cos(PI/180 * (delta_long)) - EARTH_RADIUS/GEO_ORBIT_RADIUS)/sqrt(1 - cos(PI/180 * latitude) * cos(PI/180 * (delta_long))));
	if (!(terminalElevation >= 0.000001 && terminalElevation <=90)) {
		terminalElevation = -99999;
	}
	return terminalElevation;
}

double Mobile_Terminal::getTerminalAzimuth() {
	double delta_long;
	double a;
	delta_long = orbitalPosition - longitude;
	a = std::abs(180/PI * atan(tan(PI/180 * std::abs(delta_long))/sin(PI/180 * latitude)));
	getTerminalElevation();
	if (terminalElevation >= 0.000001 && terminalElevation <=90) {
//		cout << "###########################################" << endl;
		if (latitude >= 0 && delta_long >= 0) {
			terminalAzimuth = 180 - a;
//			cout << "111111" << endl;
		} else if (latitude >= 0 && delta_long < 0) {
			terminalAzimuth = 180 + a;	
//			cout << "222222" << endl;
		} else if (latitude < 0 && delta_long > 0) {
			terminalAzimuth = a;
//			cout << "333333" << endl;
		} else if (latitude < 0 && delta_long < 0) {
			terminalAzimuth = 360 - a;
//			cout << "444444" << endl;
		}
//		cout << "###########################################" << endl;
	} else {
		terminalAzimuth = -99999;
	}	
	return terminalAzimuth;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the antenna width in the direction of the satellite in use.  The antenna width is necessary for			/////
/////		calculating the antenna discrimination (i.e. gain rolloff) to the neighboring (interfering) satellites for ASI computations.			/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

double Mobile_Terminal::getEffectiveWidth() {
	double temp_var1;
	double temp_var2;
	double temp_var3;
	double temp_var4;
	
	temp_var1 = pow(cos(PI/180 * getTerminalAzimuth())/(width/2), 2) + pow(sin(PI/180 * getTerminalAzimuth())/(height/2), 2);
	temp_var2 = pow(cos(PI/180 * getTerminalAzimuth()) * sin(PI/180 * getTerminalAzimuth()), 2);
	temp_var3 = pow(1/(width/2 * width/2) - 1/(height/2 * height/2), 2);
	temp_var4 = pow(sin(PI/180 * getTerminalAzimuth())/(width/2), 2) + pow(cos(PI/180 * getTerminalAzimuth())/(height/2), 2);
	
	effectiveWidth = 2 * sqrt(1/(temp_var1 - temp_var2 * temp_var3/temp_var4));
	
	return effectiveWidth;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the antenna height in the direction of the satellite in use.  The effective antenna height is 			/////
/////		necessary for calculating the antenna peak gain.																						/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

double Mobile_Terminal::getEffectiveHeight() {
	if (elevationTracking) {
		effectiveHeight = height;
	} else {
		effectiveHeight = height * sin(getTerminalElevation() * PI/180);
	}
	return effectiveHeight;
}

double Mobile_Terminal::getPeakGain(double freq) {
	peakGain = 10 * std::log10(efficiency) + 10 * std::log10(4 * PI * std::pow(10 * freq/2.9979248, 2) * PI * width/2 * getEffectiveHeight()/2);
	return peakGain;
}

double Mobile_Terminal::getPeakGain(double freq, string input) {  // for native antenna cut i.e. no adjustments for tracking
	peakGain = 10 * std::log10(efficiency) + 10 * std::log10(4 * PI * std::pow(10 * freq/2.9979248, 2) * PI * width/2 * height/2);
	return peakGain;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the antenna roll off or antenna pattern assuming a circular aperture with uniform illumination, 		/////
/////		parabolic taper (from center to edge), or parabolic squared illumination.  For the parabolic parabolic and parabolic squared			/////	
/////		illumination cases, we also have a pedestal - that is the edge illumination doesn't go to zero. 										/////		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

double Mobile_Terminal::getBesselRollOff(double freq) {			///////  bessel rolloff for  mispointed terminal
	double bessel_angle;
	if (!(mispointing == 0)) {
		bessel_angle = 2 * PI * freq * 1000/299.79248 * sqrt(getEffectiveWidth()/2 * getEffectiveWidth()/2) * sin(mispointing * PI/180);	
		double rolloff_numerator = (pow(10, pedestal/20) * (2 * integerBessel(bessel_angle,1)/bessel_angle) + (1 - pow(10,pedestal/20))/(taperType + 1) *
										pow(2, taperType + 1) * factorial(taperType + 1) * integerBessel(bessel_angle, taperType + 1)/pow(bessel_angle, taperType + 1));			
		double rolloff_denominator = (pow(10,pedestal/20) + (1 - pow(10,pedestal/20))/(taperType + 1));			
		besselRollOff = 10 * log10 (pow(rolloff_numerator / rolloff_denominator, 2));
	} else {
		besselRollOff = 0;
	}
	return besselRollOff;
}

double Mobile_Terminal::getBesselRollOff(double freq, double offAng) {		////// applicable to ASI calcs
	double bessel_angle;
	if (!(offAng == 0)) {
		bessel_angle = 2 * PI * freq * 1000/299.79248 * sqrt(getEffectiveWidth()/2 * getEffectiveWidth()/2) * sin(offAng * PI/180);
		double rolloff_numerator = (pow(10, pedestal/20) * (2 * integerBessel(bessel_angle,1)/bessel_angle) + (1 - pow(10,pedestal/20))/(taperType + 1) *
										pow(2, taperType + 1) * factorial(taperType + 1) * integerBessel(bessel_angle, taperType + 1)/pow(bessel_angle, taperType + 1));			
		double rolloff_denominator = (pow(10,pedestal/20) + (1 - pow(10,pedestal/20))/(taperType + 1));			
		besselRollOff = 10 * log10 (pow(rolloff_numerator / rolloff_denominator, 2));
	} else {
		besselRollOff = 0;
	}
	return besselRollOff;
}

double Mobile_Terminal::getBesselRollOff(double freq, double offAng, string input) {		// for native antenna cut i.e. no adjustments for az and el tracking
	double bessel_angle;
	if (!(offAng == 0)) {
		bessel_angle = 2 * PI * freq * 1000/299.79248 * sqrt(width/2 * width/2) * sin(offAng * PI/180);
		double rolloff_numerator = (pow(10, pedestal/20) * (2 * integerBessel(bessel_angle,1)/bessel_angle) + (1 - pow(10,pedestal/20))/(taperType + 1) *
										pow(2, taperType + 1) * factorial(taperType + 1) * integerBessel(bessel_angle, taperType + 1)/pow(bessel_angle, taperType + 1));			
		double rolloff_denominator = (pow(10,pedestal/20) + (1 - pow(10,pedestal/20))/(taperType + 1));			
		besselRollOff = 10 * log10 (pow(rolloff_numerator / rolloff_denominator, 2));
	} else {
		besselRollOff = 0;
	}
	return besselRollOff;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating Bessel functions. By changing the the number of recursions, "even_check", you can increase the number 	/////
/////		of beam widths or distance for which this function is accurate.  The function provides results that are within 1% of the MS Excel 		/////
/////		Bessel function for across approximately:																								/////
/////			+/-4 beamwidths for even_count = 4																								/////
/////			+/-10 beamwidths for even_count = 40																								/////
/////			+/-25 beamwidths for even_count = 100																								/////
/////	The general (approximate) rule of thumb is that even_count/4 = number of beam widths.														/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double Mobile_Terminal::integerBessel(double bessel_input, int bessel_order)  {
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
	even_check = 32;
	
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
		
int Mobile_Terminal::factorial(int fact_input) {

	if(fact_input > 1)
		return fact_input  * factorial(fact_input  - 1);
	else
		return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the FCC required antenna envelop (29-25*log10(theta)).  												/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
double Mobile_Terminal::getFCC_OffAxisGain() {  /////	for providing FCC envelop for mispointed antenna
	double angle = std::abs(mispointing);
	
	if (angle >= 1.5 && angle <= 7) {
		FCC_OffAxisGain = 29 - 25 * log10(angle);
	} else if (angle > 7 && angle <= 9.2) {
		FCC_OffAxisGain = 8;
	} else if (angle > 9.2 && angle <= 48) {
		FCC_OffAxisGain = 32 - 25 * log10(angle);
	} else if (angle > 48 && angle <= 85) {
		FCC_OffAxisGain = -10;
	} else {
		FCC_OffAxisGain = 0;
	}
	return FCC_OffAxisGain;
}
	
double Mobile_Terminal::getFCC_OffAxisGain(double offAng) {			/////  for providing FCC envelop pattern for ASI calcs
	double angle = std::abs(offAng);	
	if (angle >= 1.5 && angle <= 7) {
		FCC_OffAxisGain = 29 - 25 * log10(angle);
	} else if (angle > 7 && angle <= 9.2) {
		FCC_OffAxisGain = 8;
	} else if (angle > 9.2 && angle <= 48) {
		FCC_OffAxisGain = 32 - 25 * log10(angle);
	} else if (angle > 48 && angle <= 85) {
		FCC_OffAxisGain = -10;
	} else {
		FCC_OffAxisGain = 0;
	}
	return FCC_OffAxisGain;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating antenna gain roll-off as a function of effective antenna width and offset angle.  We are using			/////
/////		the formulas from the ITU for parabolic reflector antennas																				/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
double Mobile_Terminal::getITU_OffAxisGain(double freq) { 		/////	ITU rolloff for mispointed antenna

	double lambda = 0.29979248/freq;
	double wave_number = 2 * PI * freq * 1000 / 299.79248;
//	double effW = Mobile_Terminal::getEffectiveWidthForASI();
	double effW = getEffectiveWidth();
	double d_over_lambda = effW/lambda;
	double G_sidelobe1;
	double pk_gain = getPeakGain(freq);
	double angle = std::abs(mispointing);
	
	if (d_over_lambda < 100) {
		G_sidelobe1 = -21 + 25 * log10(d_over_lambda);
	} else {
		G_sidelobe1 = -1 + 15 * log10(d_over_lambda);
	}
	
	double phi_m = 20/d_over_lambda * sqrt(pk_gain - G_sidelobe1);
	double phi_r = 95/d_over_lambda;
	double phi_b = pow(10, 1.36);
	double arg = wave_number * effW/2 * sin(angle * PI/180);
	
//			cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
//			cout << "phi m: " << phi_m << endl;
//			cout << "phi r: " << phi_r << endl;
//			cout << "phi b: " << phi_b << endl;
//			cout << "k: " << wave_number << endl;
//			cout << "off axis angle: " << angle << endl;
//			cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
	
	if (angle >= 0 && angle < phi_m) {
		ITU_OffAxisGain = pk_gain - 0.0025 * pow(effW/lambda * angle, 2);
	} else if (angle >= phi_m && angle < phi_r) {
		ITU_OffAxisGain = 29 - 25 * log10(angle);
	} else if (angle >= phi_r && angle < phi_b) {
		ITU_OffAxisGain= 29 - 25 * log10(angle);
	} else if (angle > phi_b && angle < 70) {
		ITU_OffAxisGain = -5;
	} else if (angle >= 70 && angle < 180) {
		ITU_OffAxisGain = 0;
	}		
	return ITU_OffAxisGain;
}

double Mobile_Terminal::getITU_OffAxisGain(double freq, double offAng) { 		/////	for providing ITU envelop pattern
	double lambda = 0.29979248/freq;
	double wave_number = 2 * PI * freq * 1000 / 299.79248;
	double effW = getEffectiveWidth();
	double d_over_lambda = effW/lambda;
	double G_sidelobe1;
	double pk_gain = getPeakGain(freq);
	double angle = std::abs(offAng);
	
	if (d_over_lambda < 100) {
		G_sidelobe1 = -21 + 25 * log10(d_over_lambda);
	} else {
		G_sidelobe1 = -1 + 15 * log10(d_over_lambda);
	}
	
	double phi_m = 20/d_over_lambda * sqrt(pk_gain - G_sidelobe1);
	double phi_r = 95/d_over_lambda;
	double phi_b = pow(10, 1.36);
	double arg = wave_number * effW/2 * sin(angle * PI/180);
	
//			cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
//			cout << "phi m: " << phi_m << endl;
//			cout << "phi r: " << phi_r << endl;
//			cout << "phi b: " << phi_b << endl;
//			cout << "k: " << wave_number << endl;
//			cout << "off axis angle: " << angle << endl;
//			cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
	
	if (angle >= 0 && angle < phi_m) {
		ITU_OffAxisGain = pk_gain - 0.0025 * pow(effW/lambda * angle, 2);
	} else if (angle >= phi_m && angle < phi_r) {
		ITU_OffAxisGain = 29 - 25 * log10(angle);
	} else if (angle >= phi_r && angle < phi_b) {
		ITU_OffAxisGain= 29 - 25 * log10(angle);
	} else if (angle > phi_b && angle < 70) {
		ITU_OffAxisGain = -5;
	} else if (angle >= 70 && angle < 180) {
		ITU_OffAxisGain = 0;
	}		
	return ITU_OffAxisGain;
}

double Mobile_Terminal::getITU_OffAxisGain(double freq, double offAng, string input) { 		/////	for providing ITU envelop pattern for ASI calcs - serves as a check for Bessel rolloff
	double lambda = 0.29979248/freq;
	double wave_number = 2 * PI * freq * 1000 / 299.79248;
//	double effW = getEffectiveWidth();
	double effW = width;
	double d_over_lambda = effW/lambda;
	double G_sidelobe1;
	double pk_gain = getPeakGain(freq, "native");
	double angle = std::abs(offAng);
	
	if (d_over_lambda < 100) {
		G_sidelobe1 = -21 + 25 * log10(d_over_lambda);
	} else {
		G_sidelobe1 = -1 + 15 * log10(d_over_lambda);
	}
	
	double phi_m = 20/d_over_lambda * sqrt(pk_gain - G_sidelobe1);
	double phi_r = 95/d_over_lambda;
	double phi_b = pow(10, 1.36);
	double arg = wave_number * effW/2 * sin(angle * PI/180);
	
//			cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
//			cout << "phi m: " << phi_m << endl;
//			cout << "phi r: " << phi_r << endl;
//			cout << "phi b: " << phi_b << endl;
//			cout << "k: " << wave_number << endl;
//			cout << "off axis angle: " << angle << endl;
//			cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
	
	if (angle >= 0 && angle < phi_m) {
		ITU_OffAxisGain = pk_gain - 0.0025 * pow(effW/lambda * angle, 2);
	} else if (angle >= phi_m && angle < phi_r) {
		ITU_OffAxisGain = 29 - 25 * log10(angle);
	} else if (angle >= phi_r && angle < phi_b) {
		ITU_OffAxisGain= 29 - 25 * log10(angle);
	} else if (angle > phi_b && angle < 70) {
		ITU_OffAxisGain = -5;
	} else if (angle >= 70 && angle < 180) {
		ITU_OffAxisGain = 0;
	}		
	return ITU_OffAxisGain;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the antenna gain roll off profile to use for ASI calculations.  The ASI calculations are more			/////
/////		applicable to small to moderate size terminals (Ku/Ka/V band terminals from 30 cm to 2.4 m) than to gateway antennas (Ku/Ka/V			/////
/////		hub antennas with diamter > 7 m).  We use the bessel function rolloff for small off-axis angles and then we use the worse of the		/////
/////		bessel function estimate or the FCC 25.209 off-axis mask.  Since the Bessel function is more accurate than the ITU pattern,				/////
/////		we will use only the (worse) of the FCC and Bessel function rolloff calcs.  You can use the antennaPatternDisplay function to compare	/////
/////		the FCC mask, ITU small antenna mas, and the Bessel rolloff.																			/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double Mobile_Terminal::getCompositeOffAxisGain(double freq, double angle) {
//	double spacing;
//	double angle = getTopocentricAngle(spacing);
	double off_axis_envelop;
	double fcc_envelop;
	double  pk_gain = getPeakGain(freq);
	double bessel_envelop;
//	double itu_envelop;
	
	fcc_envelop = getFCC_OffAxisGain(angle);
	bessel_envelop = pk_gain + getBesselRollOff(freq, angle);
//	itu_envelop = getITU_OffAxisGain(freq, angle);
	
	if (std::abs(angle) < 1.5) {
		off_axis_envelop = bessel_envelop;
	} else {
		off_axis_envelop = std::max(fcc_envelop, bessel_envelop);
//		off_axis_envelop = std::max(off_axis_envelop, itu_envelop);
	}
	return off_axis_envelop;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the topocentric angle sub-tended by the satellite in use, the aircraft terminal, and the neighboring	/////
/////		(i.e. interfering) satellites with spacing "spacing" degrees away from the satellite in use.										 	/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double Mobile_Terminal::getTopocentricAngle(double satSpacing) {
	double delta_long = longitude - orbitalPosition;
	double topo_numerator;
	double topo_denominator;
	double topocentricAngle;			
	
	topo_numerator = (GEO_ORBIT_RADIUS - EARTH_RADIUS * cos(latitude * PI/180) * cos(delta_long * PI/180)) * (GEO_ORBIT_RADIUS *
					cos(satSpacing * PI/180) - EARTH_RADIUS * cos(latitude * PI/180) * cos(delta_long * PI/180)) +
					(-EARTH_RADIUS * sin(latitude * PI/180)) * (-EARTH_RADIUS * sin(latitude * PI/180)) + (-EARTH_RADIUS * cos(latitude * PI/180) *
					sin(delta_long * PI/180)) * (GEO_ORBIT_RADIUS * sin(satSpacing * PI/180) - EARTH_RADIUS * cos(latitude * PI/180) * sin(delta_long * PI/180));
	
	
	topo_denominator = sqrt(pow(GEO_ORBIT_RADIUS, 2) + pow(EARTH_RADIUS, 2) - 2 * GEO_ORBIT_RADIUS * EARTH_RADIUS * cos(latitude * PI/180) *
						cos(delta_long * PI/180)) * sqrt(pow(GEO_ORBIT_RADIUS, 2) + pow(EARTH_RADIUS, 2) - 2 * GEO_ORBIT_RADIUS * EARTH_RADIUS *
						cos(latitude * PI/180) * (cos(delta_long * PI/180) * cos(satSpacing * PI/180) + sin(delta_long * PI/180) * sin(satSpacing * PI/180)));
			
	topocentricAngle = 180/PI * acos(topo_numerator/topo_denominator);
	
	return topocentricAngle;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are using the getTopocentricAngle and the compositeOffAxisGain functions to populate the								/////
/////		ASI_AnglesAndRollOff structure																											/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Mobile_Terminal::calculateASI_AnglesAndRollOff(double freq, Neighbor_Satellite neighbor) {
	double satSpacing;
	double topo_angle;
	double envelop;
	ASI_AngleAndRollOff temp;
	int limit;
	limit = neighbor.getNumberOfSatellites();
	
	// for satellites to the west and east of the desired satellite
	for (int i = -limit; i <= limit; i++) {
		if (i < 0 || i > 0) {
			satSpacing = neighbor.getSpacing() * i;
			topo_angle = getTopocentricAngle(satSpacing);
			envelop = getPeakGain(freq) - getCompositeOffAxisGain(freq, topo_angle); // choosing worse of FCC or Bessel rolloff calcs
			temp = {satSpacing, topo_angle, envelop};
			topoAnglesAndRollOff.push_back(temp);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  This function is for displaying native aircraft antenna patterns	- not taking into account effective width and height						////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mobile_Terminal::antennaPatternDisplay(double freq) {

	double antenna_off_axis_array[181];
	double antenna_bessel_rolloff_array[181];
	double antenna_FCC_rolloff_array[181];
	double antenna_ITU_rolloff_array[181];
	double antenna_composite_rolloff_array[181];
		
	printf(" theta      rolloff  \n");
	printf("---------------------\n");

	int i = 0;
	for (int j = -90; j < 91; j++) {		
		antenna_off_axis_array[i] = j * 0.2;
//		antenna_composite_rolloff_array[i] = antennaEnvelopCalculator(antenna_off_axis_array[i], antenna_width, antenna_height, antenna_effic,  // instead of antenna_height, should we use effective_height?
//															effective_width, freq, antenna_pedestal, antenna_taper_type);
		antenna_bessel_rolloff_array[i] = getPeakGain(freq, "native") + getBesselRollOff(freq, antenna_off_axis_array[i], "native");		
		antenna_FCC_rolloff_array[i] = getFCC_OffAxisGain(antenna_off_axis_array[i]);
		antenna_ITU_rolloff_array[i] =	getITU_OffAxisGain(freq, antenna_off_axis_array[i], "native");
		if (std::abs(antenna_off_axis_array[i]) < 1.5) {
			antenna_composite_rolloff_array[i] = antenna_bessel_rolloff_array[i];
		} else {
			antenna_composite_rolloff_array[i] = std::max(antenna_FCC_rolloff_array[i], antenna_bessel_rolloff_array[i]);
			antenna_composite_rolloff_array[i] = std::max(antenna_composite_rolloff_array[i], antenna_ITU_rolloff_array[i]);
		}
		i++;
	}

	for (int i = 0; i < 181; i++) {
		printf("%4.1f     %11.7f 	%11.7f      %11.7f		%11.7f\n",antenna_off_axis_array[i], antenna_composite_rolloff_array[i],
					antenna_bessel_rolloff_array[i], antenna_FCC_rolloff_array[i], antenna_ITU_rolloff_array[i]);
	}
}


string Mobile_Terminal::toString() {
	return "Temporary Output String";
}
