#ifndef NEIGHBOR_SATELLITE_H
#define NEIGHBOR_SATELLITE_H

#include <cmath>
#include "Defines.hpp"

class Neighbor_Satellite {

private:
	int numberOfSatellites;
    double spacing;
    double EIRP_Density;
	double EIRP;
	double bandwidth;
	double topocentricAngle; // might reside in Mobile_Terminal class
	double G_Over_T;
	double SFD;
	vector<double> spacings;
	
public: 
	Neighbor_Satellite (int numOfSat, double spc, double eirp, double bw, double g_over_t, double sfd);
	
	Neighbor_Satellite() {};
	
	int getNumberOfSatellites();
	double getSpacing();
	double getEIRP_Density();
	double getEIRP();
	double getBandwidth();
	double getTopocentricAngle(); // might reside in Mobile_Terminal class
	double getG_Over_T();
	double getSFD();
};

#endif

	
