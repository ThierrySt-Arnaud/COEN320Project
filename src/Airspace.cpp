/*
 * Airspace.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include "Airspace.h"
#include <iostream>
using namespace std;

Airspace::Airspace() {
	// TODO Auto-generated constructor stub

}

Airspace::~Airspace() {
}


void Airspace::addAircraft(Hit hit) {
	pthread_mutex_lock(&airspace_mutex);
	airspace.push_back(hit);
	pthread_mutex_unlock(&airspace_mutex);
}

vector<Hit> Airspace::getAircrafts(){
	pthread_mutex_lock(&airspace_mutex);
	vector<Hit> outputAirspace = airspace;
	pthread_mutex_unlock(&airspace_mutex);
	return outputAirspace;
}

void remAircraft(vector<Hit> vector, int id) {

	/*for(unsigned int i=0; i<vector.size(); i++) {
		if(vector[i].getId() == id) {
			vector.erase(vector.begin()+i);
		}
	}*/
}

void editAircraft(vector<Hit> vector, int id, int x, int y, int z) {

	/*for(unsigned int i=0; i<vector.size(); i++) {
		if(vector[i].getId() == id) {
			vector[i].setCoordinates(x, y, z);
		}
	}*/

}

void display(vector<Hit> vector) {
	for(Hit x : vector) {
				cout << "Aircraft_id: " << x.getId() << endl;
				cout << "Coordinates: (" << x.getXCoordinates() << ',' << x.getYCoordinates() << ',' << x.getZCoordinates() << ')' << endl;
			}
	cout << endl;
}
