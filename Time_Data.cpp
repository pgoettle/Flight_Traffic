                                               #include "Time_Data.hpp"	

Time_Data::Time_Data (string t) :
	time(t)
{
    flightIDs = vector<int>();
    flightLocationIndeces = vector<int>();
}

void Time_Data::addFlightEntry (int flightID, int index) {
    flightIDs.push_back(flightID);
    flightLocationIndeces.push_back(index);        
}

int Time_Data::getFlightID (int location) {
    return flightIDs.at(location);
}

int Time_Data::getFlightLocationIndex (int location) {
    return flightLocationIndeces.at(location);
}

int Time_Data::getNumEntries() {
    return flightIDs.size();
}

string Time_Data::getTime() { return time; }
                                          