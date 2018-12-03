/*
 * UpdateAirspace.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: Jad
 */

#include <iostream>
#include <time.h>
#include "UpdateAirspace.h"
#include "Hit.h"

using namespace std;


UpdateAirspace::UpdateAirspace(Airspace* airspace, pthread_attr_t* threadAttr) {
	this->airspace = airspace;
	this->threadAttr = threadAttr;
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
		for(Hit x : airspace->getAircrafts()) {
			x.setData(x.getId(), x.getSpeedx(), x.getSpeedy(), x.getSpeedz(), x.getLocationx()+x.getSpeedx(), x.getLocationy()+x.getSpeedy(), x.getLocationz()+x.getSpeedz(), x.getEntryTime());
		}
		for(Hit y : airspace->getAircrafts()) {
			cout << '(' << y.getId() << ',' << y.getSpeedx() << ',' << y.getSpeedy() << ',' << y.getSpeedz() << ',' << y.getLocationx() << ',' << y.getLocationy() << ',' <<y.getLocationz() << ',' << y.getEntryTime() << ')' << endl;
		}
		clock_gettime(CLOCK_MONOTONIC, &timeout);
		timeout.tv_sec += 1;
		pthread_mutex_timedlock_monotonic(&updateMutex, &timeout);
	}

	return nullptr;
}

void UpdateAirspace::kill(){
	killFlag = true;
	pthread_mutex_unlock(&updateMutex);
	pthread_join(updateAirspace, NULL);
}
