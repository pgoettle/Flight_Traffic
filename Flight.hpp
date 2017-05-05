#ifndef FLIGHT_H
#define FLIGHT_H

#include "Flight_Location.hpp"
#include "Defines.hpp"

class Flight {

private: 
	vector<Flight_Location> dataEntries;
	int flightID;
    string call;
    string op;
    string model;
    string manufacturer;
    string source;
    string destination;
	string icao;
	string type;
	string species;

public:
	Flight(int id, string cll, string o, string mdl, string man, string from, string to, string i, string t, string s);
	Flight ();
	void appendDataEntry (Flight_Location flightEntry);
	Flight_Location getDataEntry (int location);
	int getDataEntriesSize ();
	string toString();
	string noLocationDataToString();
	int getFlightID();
	string getCall();
	string getOp();
	string getModel();
	string getManufacturer();
	string getSource();
	string getDestination();
	string getICAO();
	string getType();
	string getSpecies();
};

#endif
