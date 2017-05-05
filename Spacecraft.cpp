#include "Spacecraft.hpp"

const double EARTH_RADIUS = 6378.137;
const long GEO_ALTITUDE = 35786;
const double PI = 3.14159265358979;
const double GEO_ORBIT_RADIUS = GEO_ALTITUDE + EARTH_RADIUS;

Spacecraft::Spacecraft (string satName, double orbPosn, double westCovgExt, double eastCovgExt, double northCovgExt, double southCovgExt,double covgTilt, double cellDia, double primeAzCtr,
						 double primeElCtr, double satRxFreq, double satTxFreq, double satPtgErr) {
	
	satelliteName = satName;
	orbitalPosition = orbPosn;
	westCoverageExtent = westCovgExt;
	eastCoverageExtent = eastCovgExt;
	northCoverageExtent = northCovgExt;
	southCoverageExtent = southCovgExt;
	coverageTiltAngle = covgTilt;
	cellSize = cellDia;
	primeCellAzimuthCenter = primeAzCtr;
	primeCellElevationCenter = primeElCtr;
	spacecraftRxFreq = satRxFreq;
	spacecraftTxFreq = satTxFreq;
	spacecraftPtgError = satPtgErr;
	
//	int numberOfRows = 0;
//	int row_1_type = 0;
//	int numberOfCellsRow_1 = 0;
//	int numberOfCellsRow_2 = 0;
//	int numberOfCells = 0;
//	double cell_1_Row_1_Elevation = 0;
//	double cell_1_Row_1_Azimuth = 0;
//	double cell_1_Row_2_Azimuth = 0;
	
//	vector<Cell> cells;
};



//double Spacecraft::getAzimuth() { return azimuth; }
//double Spacecraft::getElevation() { return elevation; }
double Spacecraft::getWestCoverageExtent() { return westCoverageExtent; }
double Spacecraft::getEastCoverageExtent() { return eastCoverageExtent; }
double Spacecraft::getNorthCoverageExtent() { return northCoverageExtent; }
double Spacecraft::getSouthCoverageExtent() { return southCoverageExtent; }
double Spacecraft::getCoverageTiltAngle() { return coverageTiltAngle; }
double Spacecraft::getCellSize() { return cellSize; }
double Spacecraft::getPrimeCellAzimuthCenter() { return primeCellAzimuthCenter; }
double Spacecraft::getPrimeCellElevationCenter() { return primeCellElevationCenter; }
double Spacecraft::getSpacecraftRxFreq() { return spacecraftRxFreq; }
double Spacecraft::getSpacecraftTxFreq() { return spacecraftTxFreq; }
double Spacecraft::getCoverageAreaSolidAngle() { return (eastCoverageExtent - westCoverageExtent) * (northCoverageExtent - southCoverageExtent); }
int Spacecraft::getNumberOfRows() { return numberOfRows; }
int Spacecraft::getNumberOfCells() { return numberOfCells; }
double Spacecraft::getCell_1_Row_1_Azimuth() { return cell_1_Row_1_Azimuth; }
double Spacecraft::getCell_1_Row_1_Elevation() { return cell_1_Row_1_Elevation; }
double Spacecraft::getCell_1_Row_2_Azimuth() { return cell_1_Row_2_Azimuth; }
int Spacecraft::getNumberOfCellsRow_1() {return numberOfCellsRow_1; }
int Spacecraft::getNumberOfCellsRow_2() {return numberOfCellsRow_2; }
int Spacecraft::getRow_1_Type() {return row_1_Type; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  This function is for calculating terminal azimuth and elevation (in spacecraft az and el coordinates) from the terminal						/////
/////		latitude and longitude  		 																										/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Spacecraft::latLongToSpacecraftAzElCalculator(double lat, double longitude, double& az, double& el) {			
	double deltaLong = longitude - orbitalPosition;
	double azimuthNumerator = sin(PI/180 * (deltaLong));
	double azimuthDenominator = (GEO_ORBIT_RADIUS/EARTH_RADIUS/cos(PI/180 * lat) - cos(PI/180 * (deltaLong)));
	az = 180/PI * atan(azimuthNumerator/azimuthDenominator);
	
	double elevationNumerator = EARTH_RADIUS * sin(PI/180 * lat);
	double elevationDenominator = sqrt(pow(EARTH_RADIUS, 2) + pow(GEO_ORBIT_RADIUS, 2) - 2 * EARTH_RADIUS * GEO_ORBIT_RADIUS
									   * cos(PI/180 * lat) * cos(PI/180 * (deltaLong)));
	el = 180/PI * asin(elevationNumerator/elevationDenominator);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	This function is for calculating the boresight/pk gain az and el for the cells on a uniform grid.					 			/////////////
/////  		The function also computes the number of rows and cells in the spacecraft coverage (az/el) window.							/////////////
/////		The function also computes the number of cells in Row1 and Row2.															/////////////
/////		This function may be superceded in the future by the user directly entering the cell boresight data. 						/////////////
/////		The function works only for a uniform grid.  																				/////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Spacecraft::cellCenterCoordinateCalculations() {
	double tempAz, tempEl;
	// if non-sensical or out-of-limit values for prime cell location are entered, then we'll use the center of the (az/el) window to calculate prime cell coordinates
	if (primeCellAzimuthCenter > eastCoverageExtent || primeCellAzimuthCenter < westCoverageExtent || primeCellElevationCenter > northCoverageExtent || primeCellElevationCenter < southCoverageExtent) {
		primeCellAzimuthCenter = (eastCoverageExtent + westCoverageExtent)/2;
		primeCellElevationCenter = (northCoverageExtent + southCoverageExtent)/2;
	}
	
	double cellCenterToCenterSpacing = 2 * cellSize/2 * cos(PI/180 * 30);
	double cellRowSpacing = cellCenterToCenterSpacing * sin(PI/180 * 60);
				
	int num_cells_to_west_of_prime_cell_in_prime_cell_row = round(std::abs(primeCellAzimuthCenter - westCoverageExtent) / cellCenterToCenterSpacing);
	int num_cells_to_east_of_prime_cell_in_prime_cell_row = round(std::abs(eastCoverageExtent - primeCellAzimuthCenter) / cellCenterToCenterSpacing);
	int num_cells_in_prime_cell_row = num_cells_to_west_of_prime_cell_in_prime_cell_row + num_cells_to_east_of_prime_cell_in_prime_cell_row + 1;

	int num_cells_to_west_of_prime_cell_in_row_above_prime_cell = ceil(std::abs(primeCellAzimuthCenter - westCoverageExtent) / cellCenterToCenterSpacing);
	int num_cells_to_east_of_prime_cell_in_row_above_prime_cell = ceil(std::abs(eastCoverageExtent - primeCellAzimuthCenter) / cellCenterToCenterSpacing);
	int num_cells_in_row_above_prime_cell = num_cells_to_west_of_prime_cell_in_row_above_prime_cell + num_cells_to_east_of_prime_cell_in_row_above_prime_cell;

	int num_rows_to_north_of_prime_cell = round(std::abs(northCoverageExtent - primeCellElevationCenter) / cellRowSpacing);
	int num_rows_to_south_of_prime_cell = round(std::abs(primeCellElevationCenter - southCoverageExtent) / cellRowSpacing);
	numberOfRows = num_rows_to_north_of_prime_cell + num_rows_to_south_of_prime_cell + 1;
//		std::cout << "#rows north of prime cell: " << num_rows_to_north_of_prime_cell << endl;
//		std::cout << "#rows south of prime cell: " << num_rows_to_south_of_prime_cell << endl;
	
	double az_far_west_cell_in_prime_cell_row = primeCellAzimuthCenter - num_cells_to_west_of_prime_cell_in_prime_cell_row * cellCenterToCenterSpacing;
	double az_far_west_cell_in_row_above_prime_cell = primeCellAzimuthCenter - num_cells_to_west_of_prime_cell_in_row_above_prime_cell * cellCenterToCenterSpacing
			+ cellCenterToCenterSpacing/2;
	
	cell_1_Row_1_Elevation = num_rows_to_north_of_prime_cell * cellRowSpacing + primeCellElevationCenter;
	
	row_1_Type = num_rows_to_north_of_prime_cell % 2;
	if (row_1_Type == 0) {
		cell_1_Row_1_Azimuth = az_far_west_cell_in_prime_cell_row;
		cell_1_Row_2_Azimuth = az_far_west_cell_in_row_above_prime_cell;
		numberOfCellsRow_1 = num_cells_in_prime_cell_row;
		numberOfCellsRow_2 = num_cells_in_row_above_prime_cell;
	} else {
		cell_1_Row_1_Azimuth = az_far_west_cell_in_row_above_prime_cell;
		cell_1_Row_2_Azimuth = az_far_west_cell_in_prime_cell_row;
		numberOfCellsRow_1 = num_cells_in_row_above_prime_cell;
		numberOfCellsRow_2 = num_cells_in_prime_cell_row;
	}

	if (numberOfRows % 2 == 0) {
		numberOfCells = numberOfRows/2 * numberOfCellsRow_1 + numberOfRows/2 * numberOfCellsRow_2;
	} else{
		numberOfCells = (numberOfRows + 1) / 2 * numberOfCellsRow_1 + (numberOfRows - 1)/ 2 * numberOfCellsRow_2;
	}
	std::cout << "num rows: " << numberOfRows << "   num cells: " << numberOfCells << "\n";
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////	below we are computing the az and el coordinates for each cell
	int limit;
//			cout << "sat index: " << local_sat_index << endl;
//			cout << "num rows: " << satelliteArray[local_sat_index].num_rows << endl;
	int i = 0;
	int j = 0;
	int k = 0;
	for (i = 0; i < numberOfRows; i++) {
		if (i % 2 == 0) {
			limit = numberOfCellsRow_1;
		} else {
			limit = numberOfCellsRow_2;
		}
		for (j = 0; j < limit; j++) {
			if (i % 2 == 0) {
				tempAz = cell_1_Row_1_Azimuth + j * cellCenterToCenterSpacing;
			} else {
				tempAz = cell_1_Row_2_Azimuth + j * cellCenterToCenterSpacing;
			}
			
			tempEl = cell_1_Row_1_Elevation - i * cellRowSpacing;
//			std::cout << "cell " << k << ": az: " << tempAz << "  el: " << tempEl << "\n";
			Cell tempCell(tempAz, tempEl);
//			cells.push_back(tempAz, tempEl);
			cells.push_back(tempCell);
			k++;
		}
	}
/////	For printing to the console, the cell grid (cell centers)	/////////////////////////
//			std::cout << "k: " << k << "....." << cells.size()<< "\n";
//			for (i = 0; i < cells.size(); i++) {
//				     std::cout << i << " cell ... " << " cell az: " << cells[i].getAzimuthCenter() <<
//			" ........ " << "cell el: " << cells[i].getElevationCenter() << "\n";
//			}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////	In this function we are calculating the two row numbers of the cells that are closest to the terminal/airplane/ship.					/////
/////	 	One of the rows is "abave" the terminal and the other row is "below" the terminal 													/////
/////		We are also calculating which cell in each row is closest to the terminal.															/////
/////		One cell in each row is to the "left" of the terminal and one cell is to the "right" of the terminal for a total of 4 cells.		/////
/////		We perform a sort of the cells (and the corresponding cell indices) and return the sorted offset angles and cell indices			/////
/////		We are also checking to make sure that the terminal is not outside the region that is covered by the uniform grid.					/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Spacecraft::findAndSortFourBestCells(double spacecraftAz, double spacecraftEl, string& spacecraftStatus, int& spacecraftStatusValue, 
								    int& index1, double& offset_angle_1, int& index2, double& offset_angle_2, int& index3, double& offset_angle_3,
									int& index4, double& offset_angle_4) {		
	double cellCenterToCenterSpacing = 2 * cellSize/2 * cos(PI/180 * 30);
	double cellRowSpacing = cellCenterToCenterSpacing * sin(PI/180 * 60);								   
	int spacecraftRowNumber1 = (cell_1_Row_1_Elevation - spacecraftEl)/cellRowSpacing + 1;
	int spacecraftRowNumber2;
	int spacecraftRowNumber1Cell1;
	int spacecraftRowNumber1Cell2;
	int spacecraftRowNumber2Cell1;
	int spacecraftRowNumber2Cell2;
	
	if (spacecraftRowNumber1 < 0 || spacecraftRowNumber1 > numberOfRows) {
		spacecraftStatus = "Outside Coverage Range";
		spacecraftStatusValue = 0;
		spacecraftRowNumber1 = -999;
		spacecraftRowNumber2 = -999;
	} else {
		spacecraftStatus = "Inside Coverage Range";
		spacecraftStatusValue = 1;
		if (round(spacecraftRowNumber1) == floor(spacecraftRowNumber1)) {
			spacecraftRowNumber1 = round(spacecraftRowNumber1);
			spacecraftRowNumber2 = spacecraftRowNumber1 + 1;
		} else {
			spacecraftRowNumber2 = round(spacecraftRowNumber1);
			spacecraftRowNumber1 = spacecraftRowNumber2 - 1;
		}
		
		if (spacecraftRowNumber1 == numberOfRows) {
			spacecraftRowNumber2 = spacecraftRowNumber1;
		}
		
	}
	
	if (spacecraftRowNumber1 % 2 == 0) {
		spacecraftRowNumber1Cell1 = std::abs(cell_1_Row_1_Azimuth - spacecraftAz)/cellCenterToCenterSpacing;
	} else {
		spacecraftRowNumber1Cell1 = std::abs(cell_1_Row_2_Azimuth - spacecraftAz)/cellCenterToCenterSpacing;
	}
	
	if (spacecraftRowNumber1Cell1 < 0 || spacecraftRowNumber1Cell1 > std::min(numberOfCellsRow_1,numberOfCellsRow_2)  || spacecraftStatusValue == 0) {
		spacecraftStatus = "Outside Coverage Range";
		spacecraftStatusValue = 0;
		spacecraftRowNumber1Cell1 = -999;
		spacecraftRowNumber2Cell1 = -999;
	} else {
		spacecraftStatus = "Inside Coverage Range";
		spacecraftStatusValue = 1;
		if (round(spacecraftRowNumber1Cell1) == floor(spacecraftRowNumber1Cell1)) {
			spacecraftRowNumber1Cell1 = round(spacecraftRowNumber1Cell1);
			spacecraftRowNumber1Cell2 = spacecraftRowNumber1Cell1 + 1;
		} else {
			spacecraftRowNumber1Cell2 = round(spacecraftRowNumber1Cell1);
			spacecraftRowNumber1Cell1 = spacecraftRowNumber1Cell2 - 1;
		}
		
		if (spacecraftRowNumber1 == numberOfRows) {
			spacecraftRowNumber1Cell2 = spacecraftRowNumber1Cell1;
		}
		
	}
	
	if (spacecraftRowNumber1 == numberOfRows) {
			spacecraftRowNumber2 = spacecraftRowNumber1;
			spacecraftRowNumber2Cell1 = spacecraftRowNumber1Cell1;
			spacecraftRowNumber2Cell2 = spacecraftRowNumber1Cell2;			
	} else {
		if (spacecraftRowNumber2 % 2 == 0) {
			spacecraftRowNumber2Cell1 = std::abs(cell_1_Row_1_Azimuth - spacecraftAz)/cellCenterToCenterSpacing;
		} else {
			spacecraftRowNumber2Cell1 = std::abs(cell_1_Row_2_Azimuth - spacecraftAz)/cellCenterToCenterSpacing;
		}
		if (spacecraftRowNumber2Cell1 < 0 || spacecraftRowNumber2Cell1 > std::min(numberOfCellsRow_1,numberOfCellsRow_2)  || spacecraftStatusValue == 0) {
			spacecraftStatus = "Outside Coverage Range";
			spacecraftStatusValue = 0;
			spacecraftRowNumber2Cell1 = -999;
			spacecraftRowNumber2Cell1 = -999;
		} else {
			spacecraftStatus = "Inside Coverage Range";
			spacecraftStatusValue = 1;	
			if (round(spacecraftRowNumber2Cell1) == floor(spacecraftRowNumber2Cell1)) {
				spacecraftRowNumber2Cell1 = round(spacecraftRowNumber2Cell1);
				spacecraftRowNumber2Cell2 = spacecraftRowNumber2Cell1 + 1;
			} else {
				spacecraftRowNumber2Cell2 = round(spacecraftRowNumber2Cell1);
				spacecraftRowNumber2Cell1 = spacecraftRowNumber2Cell2 - 1;
			}
		}
	}
//			cout << "spacecraft row num2 cell1: " << spacecraftRowNumber2Cell1 << endl;
//			cout << "spacecraft row num2 cell2: " << spacecraftRowNumber2Cell2 << endl;
	
//			cout << "row 1: " << spacecraftRowNumber1 << endl;
//			cout << "row 1 cells: " << satelliteArray[m].num_cells_in_row_1 << endl;
//			cout << "row 2: " << spacecraftRowNumber2 << endl;
//			cout << "row 2 cells: " << satelliteArray[m].num_cells_in_row_2 << endl;

	int k1, k2, k3, k4;	
	
	if (spacecraftRowNumber1 % 2 == 0) {
		k1 = spacecraftRowNumber1/2 * numberOfCellsRow_1 + (spacecraftRowNumber1/2 - 1) * numberOfCellsRow_2 +
				spacecraftRowNumber1Cell1;
		k3 = spacecraftRowNumber1/2 * numberOfCellsRow_1 + spacecraftRowNumber1/2 * numberOfCellsRow_2 +
				spacecraftRowNumber2Cell1;
	} else {
		k1 = (spacecraftRowNumber1 - 1)/2 * numberOfCellsRow_1 + (spacecraftRowNumber1 - 1)/2 * numberOfCellsRow_2 +
				spacecraftRowNumber1Cell1;				
		k3 = (spacecraftRowNumber1 + 1)/2 * numberOfCellsRow_1 + (spacecraftRowNumber1 - 1)/2 * numberOfCellsRow_2 +
				spacecraftRowNumber2Cell1;				
	}			
	k2 = k1 + 1;			
	k4 = k3 + 1;
					
	double cell1DeltaAz;
	double cell1DeltaEl;
	double cell2DeltaAz;
	double cell2DeltaEl;
	double cell3DeltaAz;
	double cell3DeltaEl;
	double cell4DeltaAz;
	double cell4DeltaEl;
									
	cell1DeltaAz = cells[k1].getAzimuthCenter() - spacecraftAz;
	cell1DeltaEl = cells[k1].getElevationCenter() - spacecraftEl;
	cell2DeltaAz = cells[k2].getAzimuthCenter() - spacecraftAz;
	cell2DeltaEl = cells[k2].getElevationCenter() - spacecraftEl;
	cell3DeltaAz = cells[k3].getAzimuthCenter() - spacecraftAz;
	cell3DeltaEl = cells[k3].getElevationCenter() - spacecraftEl;
	cell4DeltaAz = cells[k4].getAzimuthCenter() - spacecraftAz;
	cell4DeltaEl = cells[k4].getElevationCenter() - spacecraftEl;

	vector<angleAndIndex> temp(4);
	
	temp[0].index = k1;
	temp[0].angle = 180/PI * acos(cos(PI/180 * cell1DeltaAz) * cos(PI/180 * cell1DeltaEl));
	temp[1].index = k2;
	temp[1].angle = 180/PI * acos(cos(PI/180 * cell2DeltaAz) * cos(PI/180 * cell2DeltaEl));
	temp[2].index = k3;
	temp[2].angle = 180/PI * acos(cos(PI/180 * cell3DeltaAz) * cos(PI/180 * cell3DeltaEl));
	temp[3].index = k4;
	temp[3].angle = 180/PI * acos(cos(PI/180 * cell4DeltaAz) * cos(PI/180 * cell4DeltaEl));
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////	In this function part of the function, we are sorting (in ascending order) the cells by angle/distance from								/////
	/////		spacecraft terminal to cell center.																									/////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	double tempAngle;
	int tempIndex;
	while (!(temp[0].angle <= temp[1].angle <= temp[2].angle <= temp[3].angle)) {
		for (int i = 0; i < 3; i++) {
			if (temp[i].angle > temp[i+1].angle) {
				tempAngle = temp[i+1].angle;
				tempIndex = temp[i+1].index;
				temp[i+1].angle = temp[i].angle;
				temp[i+1].index = temp[i].index;
				temp[i].angle = tempAngle;
				temp[i].index = tempIndex;
			}
		}
	}	
	index1 = temp[0].index;
	offset_angle_1 = temp[0].angle;
	index2 = temp[1].index;
	offset_angle_2 = temp[1].angle;
	index3 = temp[2].index;
	offset_angle_3 = temp[2].angle;
	index4 = temp[3].index;
	offset_angle_4 = temp[3].angle;
}
	
string Spacecraft::toString() {
	return "Temporary Output String";
}