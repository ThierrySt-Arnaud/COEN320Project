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

void Airspace::remAircraft(int id) {

	int erase = 0;
	for(Hit x : airspace) {
		if(x.getId() == id) {
			break;
		} else {
			erase++;
		}
	}
	pthread_mutex_lock(&airspace_mutex);
	airspace.erase(airspace.begin()+erase);
	pthread_mutex_unlock(&airspace_mutex);
}

//update speed function

//update altitude function

void Airspace::updateAircrafts() {

	pthread_mutex_lock(&airspace_mutex);
	for(Hit x : airspace) {
		x.setData(x.getId(), x.getSpeedx(), x.getSpeedy(), x.getSpeedz(), x.getLocationx()+x.getSpeedx(), x.getLocationy()+x.getSpeedy(), x.getLocationz()+x.getSpeedz(), x.getEntryTime());
	}
	pthread_mutex_unlock(&airspace_mutex);
	/*for(Hit y : airspace) {
		cout << '(' << y.getId() << ',' << y.getSpeedx() << ',' << y.getSpeedy() << ',' << y.getSpeedz() << ',' << y.getLocationx() << ',' << y.getLocationy() << ',' <<y.getLocationz() << ',' << y.getEntryTime() << ')' << endl;
	}*/
}

void display(vector<Hit> vector) {
	for(Hit x : vector) {
				cout << "Aircraft_id: " << x.getId() << endl;
				cout << "Coordinates: (" << x.getXCoordinates() << ',' << x.getYCoordinates() << ',' << x.getZCoordinates() << ')' << endl;
			}
	cout << endl;
}
