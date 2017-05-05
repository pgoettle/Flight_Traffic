#ifndef MOBILE_TERMINAL_H
#define MOBILE_TERMINAL_H

#include <cmath>
#include <algorithm>
#include "Neighbor_Satellite.hpp"
#include "Defines.hpp"

class Mobile_Terminal {

private:
	string terminalName;
        double latitude;
        double longitude;
        bool elevationTracking;
	double rxFrequency;
	double txFrequency;
	double width;
	double height;
        double effectiveWidth;
        double effectiveHeight;
	double efficiency;
	double pedestal;
	int taperType;
	double orbitalPosition;
        double terminalAzimuth;
        double terminalElevation;
	double peakGain;
	double mispointing;
	double besselOffAxisGain;
	double ITU_OffAxisGain;
	double FCC_OffAxisGain;
	double besselRollOff;
        double ITU_RollOff;
        double FCC_RollOff;
	string modem;
	struct MODCOD {
                string modulation;
                double coding;
		double requiredEsN0;
	};	
//	struct ASI_AngleAndRollOff {
//		double deltaOrbitalPosition;
//		double topocentricAngle;
//              double rollOff;
//	};
//        vector<ASI_AngleAndRollOff> topoAnglesAndRollOff;
	
	double integerBessel(double bessel_input, int bessel_order);
	int factorial(int fact_input);
		
public: 
	Mobile_Terminal (string termName, bool elTrk, double lat, double lon, double rxFreq, double txFreq, double antW, double antH, double antEffic, double pdstl, int tapType, 
                         double misptg, string mdm, double orbPosn, Neighbor_Satellite neighbor);
	
	Mobile_Terminal() {};
	struct ASI_AngleAndRollOff {
		double deltaOrbitalPosition;
		double topocentricAngle;
                double rollOff;
	};
        vector<ASI_AngleAndRollOff> topoAnglesAndRollOff;	
	string getTerminalName();
        double getLatitude();
        double getLongitude();
	string getModem();  // still need to do - must add a look-up table of modems 
	string getMODCOD(); // still need to do - must add a look-up table of MODCODS (DVB-S2, DVB-S2X, and modem advertised performance/specifications)       
	double getRxFrequency();
	double getTxFrequency();
	string getDimensions();
	double getEfficiency();
	double getPedestal();
	int getTaperType();
	double getPeakGain(double freq);
        double getPeakGain(double freq, string input);
	double getMispointing();
	double getBesselOffAxisGain(double freq);
	double getBesselOffAxisGain(double freq, double offAng);
	double getITU_OffAxisGain(double freq);
	double getITU_OffAxisGain(double freq, double offAng);
        double getITU_OffAxisGain(double freq, double offAng, string input);
	double getFCC_OffAxisGain();
	double getFCC_OffAxisGain(double offAng);
	double getBesselRollOff(double freq);
	double getBesselRollOff(double freq, double offAng);
        double getBesselRollOff(double freq, double offAng, string input);
	double getITU_RollOff(double freq); // still need to do
	double getITU_RollOff(double freq, double offAng); // still need to do
	double getFCC_RollOff(); // still need to do
	double getFCC_RollOff(double offAng); // still need to do
        double getTerminalAzimuth();
        double getTerminalElevation();
        double getEffectiveWidth();
        double getEffectiveHeight();
        bool getElevationTrackingStatus();
        double getTopocentricAngle(double satSpacing);
        double getCompositeOffAxisGain(double freq, double angle);
        void calculateASI_AnglesAndRollOff(double freq, Neighbor_Satellite neighbor);
        void antennaPatternDisplay(double freq);
//        vector<double> getTopocentricAngles();
        string toString();
};

#endif
	