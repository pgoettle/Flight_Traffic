#include "Cell.hpp"

Cell::Cell(double azCtr, double elCtr) {
	azimuthCenter = azCtr;
	elevationCenter = elCtr;
}

double Cell::getAzimuthCenter() { return azimuthCenter; }
double Cell::getElevationCenter() { return elevationCenter; }

void Cell::replaceBestCellTier(Cell_Tier ct) { bestFitFlights = ct; }
void Cell::replaceSecondBestCellTier(Cell_Tier ct) { secondBestFitFlights = ct; }
void Cell::replaceThirdBestCellTier(Cell_Tier ct) { thirdBestFitFlights = ct; }
void Cell::replaceFourthBestCellTier(Cell_Tier ct) { fourthBestFitFlights = ct; }

Cell_Tier Cell::getBestCellTier() { return bestFitFlights; }
Cell_Tier Cell::getSecondBestCellTier() { return secondBestFitFlights; }
Cell_Tier Cell::getThirdBestCellTier() { return thirdBestFitFlights; }
Cell_Tier Cell::getFourthBestCellTier() { return fourthBestFitFlights; }

string Cell::toString() {
	string ret = "";
	ret += "Cell Center Azimuth = " + std::to_string(azimuthCenter) + ", ";
	ret += "Cell Center Elevation = " + std::to_string(elevationCenter) + "\n";

	ret += "\nBest Fit Flights:\n";
	ret += bestFitFlights.toString();

	ret += "\nSecond Best Fit Flights:\n";
	ret += secondBestFitFlights.toString();

	ret += "\nThird Best Fit Flights:\n";
	ret += thirdBestFitFlights.toString();

	ret += "\nFourh Best Fit Flights:\n";
	ret += fourthBestFitFlights.toString();

	return ret;
}
