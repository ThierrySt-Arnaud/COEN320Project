/*
 * Airspace.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include "Airspace.h"
#include <iostream>

Airspace::Airspace() {
	// TODO Auto-generated constructor stub

}

Airspace::~Airspace() {
	// TODO Auto-generated destructor stub
}


std::vector<Hit> Airspace::addAircraft(std::vector<Hit> vector, Hit hit) {

	vector.push_back(hit);
	return vector;
}

std::vector<Hit> Airspace::remAircraft(std::vector<Hit> vector, int id) {

	for(unsigned int i=0; i<vector.size(); i++) {
		if(vector[i].getId() == id) {
			vector.erase(vector.begin()+i);
		}
	}
	return vector;
}

std::vector<Hit> Airspace::editAircraft(std::vector<Hit> vector, int id, int x, int y, int z) {

	for(unsigned int i=0; i<vector.size(); i++) {
		if(vector[i].getId() == id) {
			vector[i].setCoordinates(x, y, z);
		}
	}

	return vector;
}

void Airspace::display(std::vector<Hit> vector) {
	for(Hit x : vector) {
				std::cout << "Aircraft_id: " << x.getId() << std::endl;
				std::cout << "Coordinates: (" << x.getXCoordinates() << ',' << x.getYCoordinates() << ',' << x.getZCoordinates() << ')' << std::endl;
			}
	std::cout << std::endl;
}
