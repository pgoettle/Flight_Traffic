#include "Neighbor_Satellite.hpp"

Neighbor_Satellite::Neighbor_Satellite (int numOfSat, double spc, double eirp, double bw, double g_over_t, double sfd) {					
	numberOfSatellites = numOfSat;	
	spacing = spc;
	EIRP = eirp;
	bandwidth = bw;
	G_Over_T = g_over_t;
	SFD = sfd;
};

int Neighbor_Satellite::getNumberOfSatellites() { return numberOfSatellites; }
double Neighbor_Satellite::getSpacing() { return spacing; }
double Neighbor_Satellite::getEIRP() { return EIRP; }
double Neighbor_Satellite::getBandwidth() { return bandwidth; }
double Neighbor_Satellite::getG_Over_T() { return G_Over_T; }
double Neighbor_Satellite::getSFD() { return SFD; }

double Neighbor_Satellite::getEIRP_Density() {
	EIRP_Density = EIRP - 10 * log10(bandwidth);
	return EIRP;
}