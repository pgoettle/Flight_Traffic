#include "Filtered_Flights.hpp"
#include "Flight_Schedule.hpp"

Filtered_Flights::Filtered_Flights()
{
	flightIDs = vector<int>();
	flightLocationIndeces = unordered_map<int, vector<int>>();
	filteredByLocation = false;
	schedule = NULL;
}

Filtered_Flights::Filtered_Flights(vector<int> ids, Flight_Schedule *s)
{
	flightIDs = ids;
	flightLocationIndeces = unordered_map<int, vector<int>>();
	filteredByLocation = false;
	schedule = s;
}

Filtered_Flights::Filtered_Flights(vector<int> ids, unordered_map<int, vector<int>> indeces, Flight_Schedule *s)
{
	flightIDs = ids;
	flightLocationIndeces = indeces;
	filteredByLocation = true;
	schedule = s;
}

void Filtered_Flights::addFlightEntry(int flightID, vector<int> indeces) {
	flightIDs.push_back(flightID);
	flightLocationIndeces[flightID] = indeces;
}

void Filtered_Flights::addFlightEntry(int flightID) {
	flightIDs.push_back(flightID);
}

int Filtered_Flights::getFlightID(int location) {
	return flightIDs.at(location);
}

vector<int> Filtered_Flights::getFlightIndeces(int id) {
	return flightLocationIndeces[id];
}

int Filtered_Flights::getNumEntries() {
	return flightIDs.size();
}

bool Filtered_Flights::isFilteredByLocation() {
	return filteredByLocation;
}

string Filtered_Flights::toString() {
	string ret = "";
	
	if (filteredByLocation) {
		for (unsigned int i = 0; i < flightIDs.size(); i++) {
			ret += schedule->getFlight(flightIDs.at(i)).noLocationDataToString();
			vector<int> indeces = flightLocationIndeces[flightIDs.at(i)];

			for (unsigned int j = 0; j < indeces.size(); j++) {
				ret += schedule->getFlight(flightIDs.at(i)).getDataEntry(indeces[j]).toString();
			}
		}
	} else {
		for (unsigned int i = 0; i < flightIDs.size(); i++) {
			ret += schedule->getFlight(flightIDs.at(i)).noLocationDataToString();
		}
	}
	return ret;
}

vector<pair<int, int>> Filtered_Flights::getFlightsAtTime(string t) {
	vector<pair<int, int>> pairs;
	Flight temp;

	if (filteredByLocation) {
		for (auto i = flightLocationIndeces.begin(); i != flightLocationIndeces.end(); ++i) {
			for (auto j = i->second.begin(); j != i->second.end(); ++j) {
				if (schedule->getFlight(i->first).getDataEntry(*j).getTime() == t) {
					pairs.push_back(std::make_pair(i->first, *j));
					break;
				}
			}
		}
	} else {
		for (unsigned int i = 0; i < flightIDs.size(); i++) {
			temp = schedule->getFlight(flightIDs.at(i));
			for (int j = 0; j < temp.getDataEntriesSize(); j++) {
				if (temp.getDataEntry(j).getTime() == t) {
					pairs.push_back(std::make_pair(i, j));
					break;
				}
			}
		}
	}
	return pairs;
}
