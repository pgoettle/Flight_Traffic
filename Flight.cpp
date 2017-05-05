#include "Flight.hpp"

string Flight::toString () {
	string returnString = "";

	//Assumes for formatting that every entry starts with an ID
	returnString += "ID=" + std::to_string(flightID);

	if (call != "")
		returnString += ";Call=" + call;

	if (op != "")
		returnString += ";Op=" + op;

	if (model != "")
		returnString += ";Mdl=" + model;

	if (manufacturer != "")
		returnString += ";Man=" + manufacturer;

	if (source != "")
		returnString += ";From=" + source;

	if (destination != "")
		returnString += ";To=" + destination;

	if (icao != "")
		returnString += ";ICAO=" + icao;

	if (type!= "")
		returnString += ";Type=" + type;

	if (species != "")
		returnString += ";Spec=" + species;

	for (unsigned int i = 0; i < dataEntries.size(); i++) {
		returnString += ";{";
		returnString += dataEntries.at(i).toString();
		returnString += "}";
	}

	return returnString;
}

string Flight::noLocationDataToString() {
	string returnString = "";

	//Assumes for formatting that every entry starts with an ID
	returnString += "ID=" + std::to_string(flightID);

	if (call != "")
		returnString += ";Call=" + call;

	if (op != "")
		returnString += ";Op=" + op;

	if (model != "")
		returnString += ";Mdl=" + model;

	if (manufacturer != "")
		returnString += ";Man=" + manufacturer;

	if (source != "")
		returnString += ";From=" + source;

	if (destination != "")
		returnString += ";To=" + destination;

	if (icao != "")
		returnString += ";ICAO=" + icao;

	if (type != "")
		returnString += ";Type=" + type;

	if (species != "")
		returnString += ";Spec=" + species;

	return returnString;
}

Flight::Flight(int id, string cll, string o, string mdl, string man, string from, string to, string i, string t, string s) :
	flightID(id), call(cll), op(o), model(mdl), manufacturer(man), source(from), destination(to), icao(i), type(t), species(s)
{
	dataEntries = vector<Flight_Location>();
}

Flight::Flight () {};

void Flight::appendDataEntry (Flight_Location flightEntry) {
    dataEntries.push_back(flightEntry);
}

Flight_Location Flight::getDataEntry (int location) {
    return dataEntries[location];
}

int Flight::getDataEntriesSize () {
    return dataEntries.size();
}

int Flight::getFlightID() { return flightID; }
string Flight::getCall() { return call; }
string Flight::getOp() { return op; }
string Flight::getModel() { return model; }
string Flight::getManufacturer() { return manufacturer; }
string Flight::getSource() { return source; }
string Flight::getDestination() { return destination; }
string Flight::getICAO() { return icao; };
string Flight::getType() { return type; };
string Flight::getSpecies() { return species; };

