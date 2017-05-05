#ifndef CELL_TIER_H
#define CELL_TIER_H

#include "Filtered_Flights.hpp"
#include "Defines.hpp"

class Flight_Schedule;

class Cell_Tier {

private:
	string time;
	int cellIndex;
	vector<int> flightIDs;
	vector<int> flightLocationIndeces;
	vector<double> angleToCenter;					//Feel Free to change these names or their types if they are wrong
	vector<double> rollOff;					//Feel Free to change these names or their types if they are wrong
	Flight_Schedule *schedule;

public:
	Cell_Tier(string t, int cindex, Flight_Schedule *s);
	Cell_Tier(string t, int cindex, Filtered_Flights filter, Flight_Schedule *s);
	Cell_Tier() { schedule = NULL; };
	void addFlightEntry(int flightID, int index);
	int getFlightID(int location);
	int getFlightLocationIndex(int location);
	double getAngleToCenter(int location);
	double getRollOff(int location);
	int getNumEntries();
	int getCellIndex();
	string getTime();
	string toString();
};

#endif

