#ifndef FILTERED_FLIGHTS_H
#define FILTERED_FLIGHTS_H

#include "Defines.hpp"

class Flight_Schedule;

class Filtered_Flights {

private:
	vector<int> flightIDs;
	unordered_map<int, vector<int>> flightLocationIndeces;
	bool filteredByLocation;
	Flight_Schedule *schedule;

public:
	Filtered_Flights(vector<int> ids, Flight_Schedule *s);
	Filtered_Flights(vector<int> ids, unordered_map<int, vector<int>> indeces, Flight_Schedule *s);
	Filtered_Flights();
	void addFlightEntry(int flightID);
	void addFlightEntry(int flightID, vector<int> indeces);
	bool isFilteredByLocation();
	vector<pair<int, int>> getFlightsAtTime(string t);
	int getFlightID(int location);
	vector<int> getFlightIndeces(int id);
	int getNumEntries();
	string toString();
};

#endif