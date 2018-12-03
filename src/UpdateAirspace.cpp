/*
 * UpdateAirspace.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: Jad
 */

#include "UpdateAirspace.h"
#include "Hit.h"
#include <iostream>
using namespace std;


UpdateAirspace::UpdateAirspace(Airspace* airspace, pthread_attr_t* threadAttr) {
	this->airspace = airspace;
	this->threadAttr = threadAttr;
	//pthread_mutexattr_init(&this->radarMutexAttr);
	//pthread_mutexattr_setprotocol(&this->radarMutexAttr,PTHREAD_PRIO_NONE);
	//pthread_mutex_init(&this->radarMutex, &this->radarMutexAttr);
}

UpdateAirspace::~UpdateAirspace() {
	this->kill();
	pthread_attr_destroy(threadAttr);
}

pthread_t UpdateAirspace::run() {
	//pthread_mutex_lock(&radarMutex);
	if (pthread_create(&updateAirspace, threadAttr, (UPDATE_FUNC_PTR) &UpdateAirspace::update, this) == 0){
			return updateAirspace;
		} else
			return -1;
}

void *UpdateAirspace::update(void *) {

	while(!killFlag) {
		for(Hit x : airspace->getAircrafts()) {
			x.setData(x.getId(), x.getSpeedx(), x.getSpeedy(), x.getSpeedz(), x.getLocationx()+x.getSpeedx(), x.getLocationy()+x.getSpeedy(), x.getLocationz()+x.getSpeedz(), x.getEntryTime());
		}
		for(Hit y : airspace->getAircrafts()) {
			cout << '(' << y.getId() << ',' << y.getSpeedx() << ',' << y.getSpeedy() << ',' << y.getSpeedz() << ',' << y.getLocationx() << ',' << y.getLocationy() << ',' <<y.getLocationz() << ',' << y.getEntryTime() << ')' << endl;
		}
	}

	return nullptr;
}

void UpdateAirspace::kill(){
	killFlag = true;
	pthread_mutex_unlock(&updateMutex);
	pthread_join(updateAirspace, NULL);
}
