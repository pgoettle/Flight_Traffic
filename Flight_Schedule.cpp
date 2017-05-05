#include "Flight_Schedule.hpp"
#include "Filtered_Flights.hpp"

string Flight_Schedule::toString() {
    string returnString = "";
    returnString += "Time Range=" + timeRange + ", ";
    returnString += "NumFlights=" + std::to_string(flightIDs.size());
	returnString += "\n";
        
    for (unsigned int i = 0; i < flightIDs.size(); i++) {
        returnString += "{" + flights.at(flightIDs.at(i)).toString() + "}\n";
    }       
        
    return returnString;
}

void Flight_Schedule::toFile(string path) {
	std::ofstream file(path);
	file << "Time Range=" << timeRange << ", ";
	file << "NumFlights=" << std::to_string(flightIDs.size());
	file << endl;
	
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		file << "{" << flights.at(flightIDs.at(i)).toString() << "}" << endl;
	}
	
	file.close();

}

void Flight_Schedule::addFlight (Flight flight) {
    flightIDs.push_back(flight.getFlightID());
	flights[flight.getFlightID()] = flight;
}

Flight Flight_Schedule::getFlight (int flightID) {
	return flights.at(flightID);
}

int Flight_Schedule::getFlightID (int flightNum) {
    return flightIDs.at(flightNum);
}

int Flight_Schedule::getNumFlights () {
    return flightIDs.size();
}

Flight_Schedule::Flight_Schedule (string day1, string day2) :
	timeRange(day1 + "-10:00:00 to " + day2 + "-10:00:00") 
{
	flightIDs = vector<int>();
	flights = unordered_map<int, Flight>();
}

Flight_Schedule::Flight_Schedule ()
{
	/*
	timeRange = "";
	flightIDs = vector<int>();
	flights = unordered_map<int, Flight>();
	times = vector<string>();
	timeEntries = unordered_map<string, Time_Data>();
	*/
}

bool Flight_Schedule::hasFlight (int flightID) {
	return flights.find(flightID) != flights.end();
}

Filtered_Flights Flight_Schedule::filterByManufacturer(string man) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		if (flights.at(flightIDs.at(i)).getManufacturer() == man) {
			ids.push_back(flightIDs.at(i));
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterByManufacturer(string man[], int numMans) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		for (int j = 0; j < numMans; j++) {
			if (flights.at(flightIDs.at(i)).getManufacturer() == man[j]) {
				ids.push_back(flightIDs.at(i));
				break;
			}
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterByOperator(string op) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		if (flights.at(flightIDs.at(i)).getOp() == op) {
			ids.push_back(flightIDs.at(i));
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterByOperator(string op[], int numOps) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		for (int j = 0; j < numOps; j++) {
			if (flights.at(flightIDs.at(i)).getOp() == op[j]) {
				ids.push_back(flightIDs.at(i));
				break;
			}
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterBySpecies(string specie) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		if (flights.at(flightIDs.at(i)).getSpecies() == specie) {
			ids.push_back(flightIDs.at(i));
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterBySpecies(string specie[], int numSpecies) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		for (int j = 0; j < numSpecies; j++) {
			if (flights.at(flightIDs.at(i)).getOp() == specie[j]) {
				ids.push_back(flightIDs.at(i));
				break;
			}
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterByModel(string model) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		if (flights.at(flightIDs.at(i)).getModel() == model) {
			ids.push_back(flightIDs.at(i));
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterByModel(string model[], int numModels) {
	vector<int> ids;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		for (int j = 0; j < numModels; j++) {
			if (flights.at(flightIDs.at(i)).getModel() == model[j]) {
				ids.push_back(flightIDs.at(i));
				break;
			}
		}
	}
	return Filtered_Flights(ids, this);
}

Filtered_Flights Flight_Schedule::filterByAzimuth(double lowerAz, double higherAz) {
	vector<int> ids;
	unordered_map<int, vector<int>> indecesMap;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		Flight f = flights.at(flightIDs.at(i));
		vector<int> indeces;
		for (int j = 0; j < f.getDataEntriesSize(); j++) {
			Flight_Location entry = f.getDataEntry(j);
			if (entry.getAzimuth() >= lowerAz && entry.getAzimuth() <= higherAz) {
				indeces.push_back(j);
			}
		}
		if (indeces.size() > 0) {
			ids.push_back(f.getFlightID());
			indecesMap[f.getFlightID()] = indeces;
		}
	}
	return Filtered_Flights(ids, indecesMap, this);
}

Filtered_Flights Flight_Schedule::filterByElevation(double lowerEl, double higherEl) {
	vector<int> ids;
	unordered_map<int, vector<int>> indecesMap;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		Flight f = flights.at(flightIDs.at(i));
		vector<int> indeces;
		for (int j = 0; j < f.getDataEntriesSize(); j++) {
			Flight_Location entry = f.getDataEntry(j);
			if (entry.getElevation() >= lowerEl && entry.getElevation() <= higherEl) {
				indeces.push_back(j);
			}
		}
		if (indeces.size() > 0) {
			ids.push_back(f.getFlightID());
			indecesMap[f.getFlightID()] = indeces;
		}
	}
	return Filtered_Flights(ids, indecesMap, this);
}

Filtered_Flights Flight_Schedule::filterByAzimuthAndElevation(double lowerAz, double higherAz, double lowerEl, double higherEl) {
	vector<int> ids;
	unordered_map<int, vector<int>> indecesMap;
	for (unsigned int i = 0; i < flightIDs.size(); i++) {
		Flight f = flights.at(flightIDs.at(i));
		vector<int> indeces;
		for (int j = 0; j < f.getDataEntriesSize(); j++) {
			Flight_Location entry = f.getDataEntry(j);
			if (entry.getAzimuth() >= lowerAz && entry.getAzimuth() <= higherAz && entry.getElevation() >= lowerEl && entry.getElevation() <= higherEl) {
				indeces.push_back(j);
			}
		}
		if (indeces.size() > 0) {
			ids.push_back(f.getFlightID());
			indecesMap[f.getFlightID()] = indeces;
		}
	}
	return Filtered_Flights(ids, indecesMap, this);
}

string Flight_Schedule::getTimeRange() { return timeRange; }