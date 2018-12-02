/*
 * RadarListener.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: Jad
 */

#include "RadarListener.h"
#include "Hit.h"
#include <string>

RadarListener::RadarListener(pthread_attr_t* threadAttr) {
	this->threadAttr = threadAttr;
}

RadarListener::~RadarListener() {
	pthread_attr_destroy(threadAttr);
}

const pthread_t* RadarListener::run() {
	if (pthread_create(&radarListener, threadAttr, (RADAR_FUNC_PTR) &RadarListener::updateAirspace, this) == 0){
			return &radarListener;
		} else
			return nullptr;

}

void RadarListener::populateAirspace(string inputFile) {

}

void *RadarListener::updateAirspace(void *) {

	return nullptr;
}
