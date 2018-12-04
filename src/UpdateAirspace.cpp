/*
 * UpdateAirspace.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: Jad
 */

#include <iostream>
#include <time.h>
#include "UpdateAirspace.h"
#include "CommServer.h"
#include "Hit.h"

using namespace std;


UpdateAirspace::UpdateAirspace(Airspace* airspace, CommServer* commServer, pthread_attr_t* threadAttr) {
	this->airspace = airspace;
	this->threadAttr = threadAttr;
	this->commServer = commServer;
	pthread_mutexattr_init(&this->updateMutexAttr);
	pthread_mutexattr_setprotocol(&this->updateMutexAttr,PTHREAD_PRIO_NONE);
	pthread_mutex_init(&this->updateMutex, &this->updateMutexAttr);
}

UpdateAirspace::~UpdateAirspace() {
	this->kill();
	pthread_attr_destroy(threadAttr);
}

pthread_t UpdateAirspace::run() {
	pthread_mutex_lock(&updateMutex);
	if (pthread_create(&updateAirspace, threadAttr, (UPDATE_FUNC_PTR) &UpdateAirspace::update, this) == 0){
			return updateAirspace;
		} else
			return -1;
}

void *UpdateAirspace::update(void *) {
	struct timespec timeout;

	while(!killFlag) {

		airspace->updateAircrafts();
		clock_gettime(CLOCK_MONOTONIC, &timeout);
		timeout.tv_sec += 1;
		pthread_mutex_timedlock_monotonic(&updateMutex, &timeout);
		outOfBounds();

	}
	return nullptr;
}

//create function for out of bounds check
void UpdateAirspace::outOfBounds() {

	vector<int> outOfBoundsAricrafts;
	vector<Hit> airspaceCpy = airspace->getAircrafts();
	for(Hit x : airspaceCpy) {
		if(x.getLocationx() > MAX_X || x.getLocationx() < 0 || x.getLocationy() > MAX_Y || x.getLocationy() < 0 || x.getLocationz() > MAX_Z || x.getLocationz() < 0) {
			outOfBoundsAricrafts.push_back(x.getId());
		}
	}

	for(int i : outOfBoundsAricrafts) {
		airspace->remAircraft(i);
	}

}

//create function for collision check

void UpdateAirspace::kill(){
	killFlag = true;
	pthread_mutex_unlock(&updateMutex);
	pthread_join(updateAirspace, NULL);
}
