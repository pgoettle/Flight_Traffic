#ifndef FLIGHT_LOCATION_H
#define FLIGHT_LOCATION_H

#include <cmath>
#include "Spacecraft.hpp"
#include "Defines.hpp"

class Flight_Location {

private: 
	string time;
    double altitude;
    double latitude;
    double longitude;
    double speed;
    int speedType;
    int grounded;
    double verticalSpeed;
	double heading;
	double azimuth;
	double elevation;

public:

	Flight_Location (string t, double alt, double lat, double lon, double spd, int spdType, int grnd, double vertSpeed, double h, double az, double el) :
		time(t), altitude(alt), latitude(lat), longitude(lon), speed(spd), speedType(spdType), grounded(grnd), verticalSpeed(vertSpeed), heading(h), azimuth(az), elevation(el) {};

	Flight_Location (string t, string alt, string lat, string lon, string spd, string spdType, string grnd, string vertSpeed, string h, Spacecraft spacecraft);

	Flight_Location () {};

	string toString();
	string getTime();
	double getLatitude();
	double getLongitude();
	double getAltitude();
	double getSpeed();
	double getVerticalSpeed();
	double getHeading();
	double getAzimuth();
	double getElevation();
	int getSpeedType();
	int isGrounded();
};

#endif
