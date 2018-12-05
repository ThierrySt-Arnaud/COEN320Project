/*
 * UpdateAirspace.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: Jad
 */

#include <iostream>
#include <sstream>
#include <time.h>
#include <array>
#include <math.h>
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
		collisionCheck();

		//vector<Hit> airspaceCpy = airspace->getAircrafts();
	}
	return nullptr;
}

//create function for out of bounds check
void UpdateAirspace::outOfBounds() {

	vector<int> outOfBoundsAircrafts;
	vector<Hit> airspaceCpy = airspace->getAircrafts();

	for(Hit h : airspaceCpy) {
		int id = h.getId();
		std::array<int, 3> loc = h.getLocation();
		if(loc[0] > MAX_X || loc[0] < 0 || loc[1] > MAX_Y || loc[1] < 0 || loc[2] > MAX_Z || loc[2] < 0) {
			outOfBoundsAircrafts.push_back(id);
		}
	}

	for(int i : outOfBoundsAircrafts) {
		airspace->remAircraft(i);
		std::stringstream newLog;
		newLog << "Flight #" << i << " left the airspace";
		CommMessage logNewPlane(LOG, newLog.str(), i);
		commServer->send(logNewPlane);
	}

}

void UpdateAirspace::collisionCheck(){
	vector<Hit> airspaceCpy = airspace->getAircrafts();
	size_t aircrafts = airspaceCpy.size();

	if (aircrafts > 1){
		for(unsigned i = 0; i < aircrafts-1; i++) {
			std::array<int, 3> iSpd = airspaceCpy[i].getSpeed();
			std::array<int, 3> iLoc = airspaceCpy[i].getLocation();
			iLoc[0] += COLL_EST_TIME*iSpd[0]; iLoc[1] += COLL_EST_TIME*iSpd[1]; iLoc[2] += COLL_EST_TIME*iSpd[2];

			for (unsigned j = i+1; j < aircrafts; j++){
				std::array<int, 3>  jSpd = airspaceCpy[j].getSpeed();
				std::array<int, 3> jLoc = airspaceCpy[j].getLocation();
				int xDist = abs(iLoc[0] - jLoc[0] + COLL_EST_TIME*jSpd[0]);
				if (xDist < MIN_DIST){
					int yDist = abs(iLoc[1] - jLoc[1] + COLL_EST_TIME*jSpd[1]);
					if (yDist < MIN_DIST){
						int zDist = abs(iLoc[2] - jLoc[2] + COLL_EST_TIME*jSpd[2]);
						if(zDist < MIN_DIST){
							if((pow(xDist,2)+pow(yDist,2)+pow(zDist,2)) < MIN_DIST_SQR){
								int iID =airspaceCpy[i].getId();
								int jID =airspaceCpy[j].getId();
								airspace->setColliding(iID,jID);
								std::stringstream newLog;
								newLog << "Minimum distance violation detected between flight #" << iID << " and flight #" << jID;
								CommMessage logCollision(LOG, newLog.str(), i);
								commServer->send(logCollision);
							}
						}
					}
				}
			}
		}
	}
}

void UpdateAirspace::kill(){
	killFlag = true;
	pthread_mutex_unlock(&updateMutex);
	pthread_join(updateAirspace, NULL);
}
