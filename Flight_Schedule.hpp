#ifndef FLIGHT_SCHEDULE_H
#define FLIGHT_SCHEDULE_H

#include <fstream>
#include "Flight.hpp"
#include "Defines.hpp"

#define endl std::endl

class Filtered_Flights;

class Flight_Schedule {

private:
	string timeRange;								//The date and time range that this object holds
    vector<int> flightIDs;							//A List of all the flight IDs
    unordered_map<int, Flight> flights;				//A Map of all the flight data

public: 
	Flight_Schedule (string day1, string day2);
	Flight_Schedule ();
	bool hasFlight (int flightID);
	void addFlight (Flight flight);
	Flight getFlight (int flightID);
	int getFlightID (int flightNum);
	int getNumFlights ();
	string toString();
	void toFile(string path);
	string getTimeRange();

	Filtered_Flights filterByManufacturer(string man);
	Filtered_Flights filterByManufacturer(string man[], int numMans);
	Filtered_Flights filterByOperator(string op);
	Filtered_Flights filterByOperator(string op[], int numOps);
	Filtered_Flights filterBySpecies(string specie);
	Filtered_Flights filterBySpecies(string specie[], int numSpecies);
	Filtered_Flights filterByModel(string model);
	Filtered_Flights filterByModel(string model[], int numModels);
	Filtered_Flights filterByAzimuth(double lowerAz, double higherAz);
	Filtered_Flights filterByElevation(double lowerEl, double higherEl);
	Filtered_Flights filterByAzimuthAndElevation(double lowerAz, double higherAz, double lowerEl, double higherEl);

};

#endif
