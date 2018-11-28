/*
 * main.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */
#include <iostream>
#include "Hit.h"
#include "Coordinates.h"
#include <vector>
using namespace std;

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

int main() {

	vector<Hit> L;

	Hit newHit;
	newHit.setId(1);
	newHit.setCoordinates(6, 4, 8);
	L = addAircraft(L, newHit);

	newHit.setId(8);
	newHit.setCoordinates(5, 5, 5);
	L = addAircraft(L, newHit);

	newHit.setId(6);
	newHit.setCoordinates(9, 2, 4);
	L = addAircraft(L, newHit);

	display(L);

	L = remAircraft(L, 8);

	display(L);

	L = editAircraft(L, 6, 2, 2, 2);

	display(L);

	return 0;
}

