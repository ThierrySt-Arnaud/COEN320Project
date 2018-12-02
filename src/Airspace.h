/*
 * Airspace.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef AIRSPACE_H_
#define AIRSPACE_H_

#include <vector>
#include "Hit.h"
using namespace std;

class Airspace {
public:
	vector<Hit> airspace;
	Airspace();
	virtual ~Airspace();

	vector<Hit> getAircrafts() { return airspace; }
	Hit getAircraft();
	void setAircraft() {}
	void addAircraft(Hit hit) { airspace.push_back(hit); };
	void removeAircraft(Hit hit);
	void editAircraft(Hit hit);
	int getSize();

	//Add mutex and locks

};

#endif /* AIRSPACE_H_ */
