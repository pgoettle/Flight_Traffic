#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <cmath>
#include <algorithm>
#include "Cell.hpp"
#include "Defines.hpp"

class Spacecraft {

private:
	string satelliteName;
	double orbitalPosition;
	double westCoverageExtent;
	double eastCoverageExtent;
	double northCoverageExtent;
	double southCoverageExtent;
	double coverageTiltAngle;
	double cellSize;
	double cellCenterToCenterSpacing;
	double cellRowSpacing;
	double primeCellAzimuthCenter;
	double primeCellElevationCenter;
	double spacecraftRxFreq;
	double spacecraftTxFreq;
	double spacecraftPtgError;
	double coverageAreaSolidAngle;
	double requiredSpacecraftCapacity;
	int numberOfCellsRow_1;
	int numberOfCellsRow_2;
	int numberOfRows;
	int numberOfCells;
	double cell_1_Row_1_Azimuth;
	double cell_1_Row_1_Elevation;
	double cell_1_Row_2_Azimuth;
	int row_1_Type;
	struct angleAndIndex {
		double angle;
		int index;				
	};
	vector<Cell> cells;
//	bool sortByAngle();


public: 
	Spacecraft (string satName, double orbPosn, double westCovgExt, double eastCovgExt, double northCovgExt, double southCovgExt,double covgTilt, double cellDia, double primeAzCtr, double primeElCtr,
				double satRxFreq, double satTxFreq, double satPtgErr);
	Spacecraft() {};
	string getSpacecraftName();
	double getSpacecraftOrbitalPosition();
	double getWestCoverageExtent();
	double getEastCoverageExtent();
	double getNorthCoverageExtent();
	double getSouthCoverageExtent();
	double getCoverageTiltAngle();
	double getCellSize();
	double getPrimeCellAzimuthCenter();
	double getPrimeCellElevationCenter();
	double getSpacecraftRxFreq();
	double getSpacecraftTxFreq();
	double getSpacecraftPointingError();
	double getCoverageAreaSolidAngle();
//	int getSpacecraftNumberOfTerminals();
//	int getSpacecraftNumberofFirstClassPassengers();
//	int getSpacecraftNumberofEconomyPlusClassPassengers();
//	int getSpacecraftNumberofEconomyClassPassengers();
//	int getRequiredSpacecraftCapacity();
	int getNumberOfCells();
	int getNumberOfRows();
	double getCell_1_Row_1_Azimuth();
	double getCell_1_Row_1_Elevation();
	double getCell_1_Row_2_Azimuth();
	int getNumberOfCellsRow_1();
	int getNumberOfCellsRow_2();
	int getRow_1_Type();
	
  	void latLongToSpacecraftAzElCalculator(double lat, double longitude, double& az, double& el);
	void cellCenterCoordinateCalculations();

	void findAndSortFourBestCells(double spacecraftAz, double spacecraftEl, string& spacecraftStatus, int& spacecraftStatusValue, 
							int& index1, double& offset_angle_1, int& index2, double& offset_angle_2, int& index3, double& offset_angle_3,
							int& index4, double& offset_angle_4);
		
	string toString();
};

#endif
