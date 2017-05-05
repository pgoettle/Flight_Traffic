#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdlib.h> 
#include <stdio.h>
#include <sstream>
#include "Flight_Schedule.hpp"
#include "Spacecraft.hpp"
#include "Antenna.hpp"
#include "Mobile_Terminal.hpp"
//#include "Neighbor_Satellite.hpp"

#define IN_FILE_PATH "/Users/pgoettle/Desktop/new_input_files/2016-06-21-0000Z.json"
#define OUT_FILE_PATH
#define string std::string
#define cout std::cout
#define endl std::endl

//Global Flight_Schedule object
static Flight_Schedule *schedule = NULL;
static Spacecraft spacecraft;

/*****************************************************************************************************************************
* findFieldType
* Returns the array index that the data belongs to so it can be added to the flight
* Parameters
*  field - A string that used to find the array index
* Returns an int which is the array index for the data, or -1 if the field is not used in the array
*****************************************************************************************************************************/
int findFieldType(string field) {
	int fieldNum;

	//if-else statements that find which location in the flightValues array the value should be placed in
	if (field == "Id" || field == "ID") {
		fieldNum = 0;
	}
	else if (field == "Call") {
		fieldNum = 1;
	}
	else if (field == "Op") {
		fieldNum = 2;
	}
	else if (field == "Mdl") {
		fieldNum = 3;
	}
	else if (field == "Man") {
		fieldNum = 4;
	}
	else if (field == "From") {
		fieldNum = 5;
	}
	else if (field == "To") {
		fieldNum = 6;
	}
	else if (field == "ICAO" || field == "Icao") {
		fieldNum = 7;
	}
	else if (field == "Type") {
		fieldNum = 8;
	}
	else if (field == "Species" || field == "Spec") {
		fieldNum = 9;
	}
	else if (field == "PosTime" || field == "Time") {
		fieldNum = 10;
	}
	else if (field == "Alt") {
		fieldNum = 11;
	}
	else if (field == "Lat") {
		fieldNum = 12;
	}
	else if (field == "Long") {
		fieldNum = 13;
	}
	else if (field == "Spd") {
		fieldNum = 14;
	}
	else if (field == "SpdType" || field == "SpdT") {
		fieldNum = 15;
	}
	else if (field == "Gnd") {
		fieldNum = 16;
	}
	else if (field == "Vsi" || field == "VSpd") {
		fieldNum = 17;
	}
	else if (field == "Trak") {
		fieldNum = 18;
	}
	else {
		fieldNum = -1;
	}

	return fieldNum;
}

/*****************************************************************************************************************************
* separateFlightValues
* Takes a string that contains all the data for a flight and separates it into an array that is used to create Flight Objects
* Parameters
*  token - A string that contains all the data for a single flight
*  flightValues[] - An array of strings that holds the pertinent data for a single flight
* Returns nothing
*****************************************************************************************************************************/
void separateFlightValues(string token, string (&flightValues)[19]) {
	std::fill_n(flightValues, 19, "");						//Sets all the flightValues to empty strings
	string field;											//Holds the current field Value                                                                           
	string value;											//Holds the current value associated with the current fie v vvld
	unsigned int open_location = 0;
	int close_location = -1;
	int open_value = 0;
	int close_value = 0;
	int fieldType;

	//Loops until all the data in the string has been read
	while (open_location < token.length()) {
		open_location = close_location + 2;
		close_location = token.find(",\"", open_location);

		if (token.find(",\"", open_location) == string::npos) {
			close_location = token.length();
			continue;
		}

		if (close_location - open_location < 0)
			continue;

		open_value = open_location;
		close_value = token.find(":", open_value);

		//Extract the field
		field = token.substr(open_value, close_value - open_value-1);

		//Find the array index
		fieldType = findFieldType(field);
		//Check if the field is needed
		if (fieldType == -1) {
			//Skips sections of unneeded data
			if (field == "Stops") {
				close_location = token.find("]", close_value + 1);
				if (token.find("]", close_value + 1) == string::npos || (close_location - token.length()) < 2) {
					open_location = token.length();
				}
			//Skips sections of unneeded data
			} else if (field == "Cos") {
				close_location = token.find("]", close_value + 1);
				if (token.find("]", close_value + 1) == string::npos || (close_location - token.length()) < 2) {
					open_location = token.length();
					//cout << open_location << endl;
				}
			}
			continue;
		}

		//Extracts the value for the field
		value = token.substr(close_value+1, close_location - (close_value+1));

		//Removes extra quotes from value
		if (value[0] == '\"') {
			value = value.substr(1, value.length() - 2);
		}

		//Add the value of the field to the correct index in the array
		flightValues[fieldType] = value;
	}
}

/*****************************************************************************************************************************
* createFlight
* Instantiates a Flight Object, along with the a Flight_Location Object, and adds it to the Flight_Schedule
* Parameters
*  id - An int for the flightID
*  time - a string that holds the time that this Flight was recorded at
*  flightValues[] - an array of strings that holds the other pertinent flight data
* Returns nothing
*****************************************************************************************************************************/
void createFlight(int id, string time, string values[19]) {
	Flight newFlight = Flight(id, values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9]);
	Flight_Location newPos = Flight_Location(values[10], values[11], values[12], values[13], values[14], values[15], values[16], values[17], values[18], spacecraft);
	newFlight.appendDataEntry(newPos);
	schedule->addFlight(newFlight);
}

/*****************************************************************************************************************************
* shouldAddData
* Returns a bool for whether the current flight should be added to the flight schedule (not used for filtering, only for
* entries that do not have the necessary information such as latitude and longitude)
* Parameters
*  values[] - An array of strings that hold all the flight Data
* Returns a bool for whether a Flight Object should be created and added to the Flight Schedule (true means add to Schedule)
*****************************************************************************************************************************/
bool shouldAddData(string(&values)[19]) {
	if (values[12] == "" || values[13] == "") {
		return false;
	} 
	return true;
}

/*****************************************************************************************************************************
* createFlightScheduleFromFile
* Reads data from an ADS-B file and instantiates and populates a Flight_Schedule Object with the data
* Parameters
*  none
* Returns nothing
*****************************************************************************************************************************/
void createFlightScheduleFromFile() {

	int start_s = clock();					//Gets the current time to measure performance
	size_t open_location = 0;
	size_t close_location = 0;
	string flightValues[19];
	
	spacecraft = Spacecraft("Satellite_101", -100.0, -3.7, 4.2, 7.2, 4.5, 0.0, 0.23, 10, 10, 30.0, 42.0, 0.0);		//Instantiates a Spacecraft object with a default azimuth and elevation of 0.0

	std::ios_base::sync_with_stdio(false);	//This is a speedup command so that the old stdio library is not synced with the new iosteam library

	std::ifstream newFile(IN_FILE_PATH);	//Opens the file to read
	string newLine;							//Holds the input from the file, line by line
	int i = 0;
	bool exit = false;

	// Check if file was successfully opened
	if (newFile.is_open()) {
		cout << "Opened the File" << endl;
		if (getline(newFile, newLine)) {	//Gets the input from the file (which is always just one line)

			//Gets to the Data
			open_location = newLine.find("[", open_location) + 1;
			open_location = newLine.find("[", open_location) + 2;

			close_location = newLine.find("},{", open_location);

			schedule = new Flight_Schedule("Start", "End");				//Instantiates a Flight Schedule for this data, using dummy strings since the time range is only 1 minute

			//Loop through the input line, going flight by flight
			while (!exit) {

				//Extracts a Flight from the data
				separateFlightValues(newLine.substr(open_location, close_location - open_location), flightValues);

				//Checks if the flight should be added to the Flight_Schedule, and adds the flight if true
				int tempID = stoi(flightValues[0]);
				flightValues[10] = "2016-06-21-0000Z";
				if (shouldAddData(flightValues))
					createFlight(tempID, flightValues[10], flightValues);

				//Finds the next piece of data
				open_location = close_location + 3;
				close_location = newLine.find("},{", open_location);

				//Checks if end of data
				if (close_location == string::npos) {
					close_location = newLine.find("}]", open_location);
					if (close_location == string::npos) {
						exit = true;
					}
				}
				i++;
			}
		}
		else {
			cout << "Could not read line" << endl;
		}
		newFile.close();
	}
	else {
		cout << "Could not open the file" << endl;
	}

	//Write the data to file
	cout << "To File" << endl;
	schedule->toFile("/Users/pgoettle/Desktop/newer_output_files/testfile");

	//Finds the 
	int stop_s = clock();
	cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << endl;
}

int main () {
//	createFlightScheduleFromFile();
	string time = "2016-06-21-0000Z";
	Spacecraft satellite = Spacecraft("Satellite_101", -100.0, -3.25, 4, 7.2, 4.2, 0.0, 0.23, 0.375, 5.5, 30.0, 12.0, 0.0);		//Instantiates a Spacecraft object with a default azimuth and elevation of 0.0
	Neighbor_Satellite Neighborhood_2_deg = Neighbor_Satellite(12, 0.1, 50, 36000000, 10, -92);		// Instantiates adjacent satellite object
	satellite.cellCenterCoordinateCalculations();
	double latitude = 37.7;
	double longitude = -78.8;
	double azimuth, elevation;
	satellite.latLongToSpacecraftAzElCalculator(latitude, longitude, azimuth, elevation);
	string spacecraftStatus;
	int spacecraftStatusValue;
	int cellNum1;
	int cellNum2;
	int cellNum3;
	int cellNum4;
	double offsetAngle1;
	double offsetAngle2;
	double offsetAngle3;
	double offsetAngle4;	
	satellite.findAndSortFourBestCells(azimuth, elevation, spacecraftStatus, spacecraftStatusValue,
								cellNum1, offsetAngle1, cellNum2, offsetAngle2, cellNum3, offsetAngle3, cellNum4, offsetAngle4);
	Antenna antenna1 = Antenna("antenna1", satellite.getSpacecraftTxFreq(), 5, 5, 0.5, -11, 2, offsetAngle1);
	Antenna antenna2 = Antenna("antenna1", satellite.getSpacecraftTxFreq(), 5, 5, 0.5, -11, 2, offsetAngle2);
	Antenna antenna3 = Antenna("antenna1", satellite.getSpacecraftTxFreq(), 5, 5, 0.5, -11, 2, offsetAngle3);
	Antenna antenna4 = Antenna("antenna1", satellite.getSpacecraftTxFreq(), 5, 5, 0.5, -11, 2, offsetAngle4);
	
	cout << "az: " << azimuth << "el: " << elevation << "\n";
	
	cout << "Closest cell is: " << cellNum1 << " with offset angle of: " << offsetAngle1 << " degrees and rolloff of: "<< antenna1.getRollOff() << " dB" << endl;
	cout << "2nd closest cell is: " << cellNum2 << " with offset angle of: " << offsetAngle2 << " degrees and rolloff of: "<< antenna2.getRollOff() << " dB" << endl;
	cout << "3rd closest cell is: " << cellNum3 << " with offset angle of: " << offsetAngle3 << " degrees and rolloff of: "<< antenna3.getRollOff() << " dB" << endl;
	cout << "4th closest cell is: " << cellNum4 << " with offset angle of: " << offsetAngle4 << " degrees and rolloff of: "<< antenna4.getRollOff() << " dB" << endl;
	
	Mobile_Terminal mbl_term = Mobile_Terminal("mbl_term1", 1, latitude, longitude, satellite.getSpacecraftRxFreq(), satellite.getSpacecraftTxFreq(), 0.9, 0.2, 0.6, -11, 2, 0.1,
												"iDirect Evolution", -100.0, Neighborhood_2_deg);
	mbl_term.antennaPatternDisplay(satellite.getSpacecraftTxFreq());
	
	mbl_term.calculateASI_AnglesAndRollOff(satellite.getSpacecraftTxFreq(), Neighborhood_2_deg);
	cout << "eff width: " << mbl_term.getEffectiveWidth() << endl;
	for (int i = 0; i < 2 * Neighborhood_2_deg.getNumberOfSatellites(); i++) {
		cout << "topo angle 1: " << mbl_term.topoAnglesAndRollOff[i].topocentricAngle << "  term roll off: " << mbl_term.topoAnglesAndRollOff[i].rollOff << endl;
	}

	if (schedule != NULL) {
		delete(schedule);
	}

	return 0;
}



