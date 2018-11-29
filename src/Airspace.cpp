/*
 * Airspace.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include "Airspace.h"

Airspace::Airspace() {
	// TODO Auto-generated constructor stub

}

Airspace::~Airspace() {
	// TODO Auto-generated destructor stub
}


vector<Hit> addAircraft(vector<Hit> vector, Hit hit) {

	vector.push_back(hit);
	return vector;
}

vector<Hit> remAircraft(vector<Hit> vector, int id) {

	for(unsigned int i=0; i<vector.size(); i++) {
		if(vector[i].getId() == id) {
			vector.erase(vector.begin()+i);
		}
	}
	return vector;
}

vector<Hit> editAircraft(vector<Hit> vector, int id, int x, int y, int z) {

	for(unsigned int i=0; i<vector.size(); i++) {
		if(vector[i].getId() == id) {
			vector[i].setCoordinates(x, y, z);
		}
	}

	return vector;
}

void display(vector<Hit> vector) {
	for(Hit x : vector) {
				cout << "Aircraft_id: " << x.getId() << endl;
				cout << "Coordinates: (" << x.getXCoordinates() << ',' << x.getYCoordinates() << ',' << x.getZCoordinates() << ')' << endl;
			}
	cout << endl;
}
