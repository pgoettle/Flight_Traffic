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


	// These functions allow you to filter the Flights based on the criteria you give
	// However right now only one filter can be applied, so it only allows filtering by a manufacturer or set of manufacturers.
	// The filters cannot be combined as of yet, but I will add that functionality soon
	// All you have to do is call schedule->filterByWhatever(...) and pass in the string or array of strings to filter by (or azimuth and elevation)
	// Then the returned Filtered_Flights Object can be used to actually assign Flights to Cells fdasfda
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
