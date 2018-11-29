/*
 * RadarListener.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include "RadarListener.h"

RadarListener::RadarListener(pthread_t* radarListener, pthread_t* airspaceMutex) {
	// TODO Auto-generated constructor stub
	this->radarListener = radarListener;
	this->airspaceMutex = airspaceMutex;
}

RadarListener::~RadarListener() {
	// TODO Auto-generated destructor stub
}

bool run(){

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

	return true;
}

bool kill(){

	return true;
}
