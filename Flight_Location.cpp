#include "Flight_Location.hpp"

#define stod std::stod
#define stoi std::stoi


string Flight_Location::toString() {
	string returnString = "";

	returnString += "Time=" + time;
        
    if (!std::isnan (altitude))
        returnString += ";Alt=" + std::to_string(altitude);
        
    if (!std::isnan (latitude))
        returnString += ";Lat=" + std::to_string(latitude);
        
    if (!std::isnan (longitude))
        returnString += ";Long=" + std::to_string(longitude);
        
    if (!std::isnan (speed))
        returnString += ";Spd=" + std::to_string(speed);
        
    if (speedType >= 0)
        returnString += ";SpdT=" + std::to_string(speedType);
        
    if (grounded != -1) {
        if (grounded == 1) returnString += ";Gnd=1";
        else if (grounded == 0) returnString += ";Gnd=0";
    }
        
    if (!std::isnan (verticalSpeed))
        returnString += ";VSpd=" + std::to_string(verticalSpeed);

	if (!std::isnan (heading))
		returnString += ";Trak=" + std::to_string(heading);

	if (!std::isnan(azimuth))
		returnString += ";Az=" + std::to_string(azimuth);

	if (!std::isnan(elevation))
		returnString += ";El=" + std::to_string(elevation);

	return returnString;
}

Flight_Location::Flight_Location (string t, string alt, string lat, string lon, string spd, string spdType, string grnd, string vertSpeed, string h, Spacecraft spacecraft) {
	time = t;

	if (alt == "")
        altitude = NAN;
    else
        altitude = stod(alt);
        
    if (lat == "")
        latitude = NAN;
    else
        latitude = stod(lat);
        
    if (lon == "")
        longitude = NAN;
    else
        longitude = stod(lon);
        
    if (spd == "")
        speed = NAN;
    else
        speed = stod(spd);
        
    if (spdType == "")
        speedType = -1;
    else
        speedType = stoi(spdType);
        
    if (grnd == "")
        grounded = -1;
    else if (grnd == "false" || grnd == "0")
        grounded = 0;
    else if (grnd == "true" || grnd == "1")
        grounded = 1;
    else
        grounded = -1;
                
    if (vertSpeed == "")
        verticalSpeed = NAN;
    else
        verticalSpeed = stod(vertSpeed);

	if (h == "")
		heading = NAN;
	else
		heading = stod(h);

	spacecraft.latLongToSpacecraftAzElCalculator(latitude, longitude, azimuth, elevation);	

}

string Flight_Location::getTime() { return time; }
double Flight_Location::getLatitude() { return latitude; }
double Flight_Location::getLongitude() { return longitude; }
double Flight_Location::getAltitude() { return altitude; }
double Flight_Location::getSpeed() { return speed; }
double Flight_Location::getVerticalSpeed() { return verticalSpeed; }
double Flight_Location::getHeading() { return heading; }
double Flight_Location::getAzimuth() { return azimuth; }
double Flight_Location::getElevation() { return elevation; }
int Flight_Location::getSpeedType() { return speedType; }
int Flight_Location::isGrounded() { return grounded; }
