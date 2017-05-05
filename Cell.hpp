#ifndef CELL_H
#define CELL_H

#include "Cell_Tier.hpp"
#include "Defines.hpp"

class Cell {

private:
	double azimuthCenter;
	double elevationCenter;
	Cell_Tier bestFitFlights;
	Cell_Tier secondBestFitFlights;
	Cell_Tier thirdBestFitFlights;
	Cell_Tier fourthBestFitFlights;

public:
	Cell(double azCtr, double elCtr);
	Cell() {};
	double getAzimuthCenter();
	double getElevationCenter();
	void replaceBestCellTier(Cell_Tier ct);
	void replaceSecondBestCellTier(Cell_Tier ct);
	void replaceThirdBestCellTier(Cell_Tier ct);
	void replaceFourthBestCellTier(Cell_Tier ct);
	Cell_Tier getBestCellTier();
	Cell_Tier getSecondBestCellTier();
	Cell_Tier getThirdBestCellTier();
	Cell_Tier getFourthBestCellTier();

	string toString();
};

#endif
