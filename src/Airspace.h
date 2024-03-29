/*
 * Airspace.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef AIRSPACE_H_
#define AIRSPACE_H_

#include <vector>
#include <pthread.h>
#include <map>
#include <array>
#include "Hit.h"
using namespace std;

class Airspace {
public:
	Airspace();
	virtual ~Airspace();

	vector<Hit> getAircrafts();
	Hit getAircraft();
	void setAircraft() {}
	void addAircraft(Hit hit);
	void remAircraft(int id);
	void setColliding(int, int);
	void updateAircrafts();
	void chgAircraftAlt(int, int);
	void chgAircraftSpd(int, std::array<int, 3>);
	int getSize();
private:
	std::map<int,Hit> airspace;
	pthread_mutex_t airspace_mutex = PTHREAD_MUTEX_INITIALIZER;
};

#endif /* AIRSPACE_H_ */
