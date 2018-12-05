/*
 * RadarListener.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: Jad
 */

#include "RadarListener.h"
#include "CommServer.h"
#include "Hit.h"
#include <string>
#include <sstream>
#include <iostream>
#include <queue>
#include <time.h>
using namespace std;

RadarListener::RadarListener(Airspace* airspace, CommServer* commServer, pthread_attr_t* threadAttr) {
	this->airspace = airspace;
	this->threadAttr = threadAttr;
	this->commServer = commServer;
	pthread_mutexattr_init(&this->radarMutexAttr);
	pthread_mutexattr_setprotocol(&this->radarMutexAttr,PTHREAD_PRIO_NONE);
	pthread_mutex_init(&this->radarMutex, &this->radarMutexAttr);
}

RadarListener::~RadarListener() {
	this->kill();
	pthread_attr_destroy(threadAttr);
}

pthread_t RadarListener::run() {
	pthread_mutex_lock(&radarMutex);
	if (pthread_create(&radarListener, threadAttr, (RADAR_FUNC_PTR) &RadarListener::populateAirspace, this) == 0){
		return radarListener;
	} else {
		pthread_mutex_unlock(&radarMutex);
		return -1;
	}
}

void *RadarListener::populateAirspace(void *) {
	struct timespec timeOut;
	queue<Hit> hitList;
	int timer = 0;
	string id, vx, vy, vz, x, y, z, eTime;
	int aircraft_id;
	int i = 0;
	string line;
	ifstream inFile;
	inFile.open(INPUT_FILE, ios::in);

	if (inFile.is_open()) {
			while(getline(inFile, line)) {
				if(line.find("int") != std::string::npos or line.find("}") != std::string::npos or line.empty()) {
					//do nothing
				} else {
					stringstream ss(line);
					getline(ss,id,',');
					getline(ss,vx,',');
					getline(ss,vy,',');
					getline(ss,vz,',');
					getline(ss,x,',');
					getline(ss,y,',');
					getline(ss,z,',');
					getline(ss, eTime,',');
					aircraft_id = stoi(id);
					if(aircraft_id == -1) {
						i--;
						aircraft_id = i;
					}
					Hit hit(aircraft_id, stoi(vx), stoi(vy), stoi(vz), stoi(x), stoi(y), stoi(z), stoi(eTime));
					hitList.push(hit);
					//airspace->addAircraft(hit);
					//cout << '(' << id << ',' << vx << ',' << vy << ',' << vz << ',' << x << ',' << y << ',' << z << ',' << eTime << ')' << endl;
				}
		}
	} else {
		cout << "File could not open." << endl;
	}

	inFile.close();

	while(!killFlag) {

		while(!hitList.empty()) {
			Hit front = hitList.front();
			int ET = timer - front.getEntryTime();
			if(ET > 0) {
				std::array<int, 3> spd = front.getSpeed();
				std::array<int, 3> loc = front.getLocation();
				loc[0] += ET*spd[0]; loc[1] += ET*spd[1]; loc[2] += ET*spd[2];
				front.setLocation(loc);
				airspace->addAircraft(front);
				stringstream newLog;
				newLog << "Flight #" << front.getId() << " entered the airspace";
				CommMessage logNewPlane(LOG, newLog.str(), front.getId());
				commServer->send(logNewPlane);
				hitList.pop();
			}
			else{
				break;
			}
		}

		clock_gettime(CLOCK_MONOTONIC, &timeOut);
		timeOut.tv_sec += 5;
		pthread_mutex_timedlock_monotonic(&radarMutex, &timeOut);
		timer += 5;
	}
	return nullptr;
}

void RadarListener::kill(){
	killFlag = true;
	pthread_mutex_unlock(&radarMutex);
	pthread_join(radarListener, NULL);
}
