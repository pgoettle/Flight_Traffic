#include "Cell_Tier.hpp"
#include "Flight_Schedule.hpp"

Cell_Tier::Cell_Tier(string t, int cindex, Flight_Schedule *s) {
	time = t;
	cellIndex = cindex;
	flightIDs = vector<int>();
	flightLocationIndeces = vector<int>();
	schedule = s;
}

Cell_Tier::Cell_Tier(string t, int cindex, Filtered_Flights filter, Flight_Schedule *s) {
	time = t;
	cellIndex = cindex;
	vector<pair<int, int>> pairs;
	pairs = filter.getFlightsAtTime(t);
	schedule = s;

	for (auto i = pairs.begin(); i != pairs.end(); ++i) {
		flightIDs.push_back(i->first);
		flightLocationIndeces.push_back(i->second);
	}
}

void Cell_Tier::addFlightEntry(int flightID, int index) {
	flightIDs.push_back(flightID);
	flightLocationIndeces.push_back(index);
}

int Cell_Tier::getFlightID(int location) {
	return flightIDs.at(location);
}

int Cell_Tier::getFlightLocationIndex(int location) {
	return flightLocationIndeces.at(location);
}

double Cell_Tier::getAngleToCenter(int location) {
	return angleToCenter.at(location);
}

double Cell_Tier::getRollOff(int location) {
	return rollOff.at(location);
}

int Cell_Tier::getNumEntries() {
	return flightIDs.size();
}

string Cell_Tier::getTime() {
	return time;
}

int Cell_Tier::getCellIndex() {
	return cellIndex;
}

string Cell_Tier::toString() {
	string ret = "";
	ret += "Time = " + time + ", Cell Index = " + std::to_string(cellIndex) + "\n";

	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		ret += schedule->getFlight(flightIDs.at(i)).noLocationDataToString();
		ret += ";{";
		ret += schedule->getFlight(flightIDs.at(i)).getDataEntry(flightLocationIndeces.at(i)).toString();
		ret += "}\n";
	}
	return ret;
}

