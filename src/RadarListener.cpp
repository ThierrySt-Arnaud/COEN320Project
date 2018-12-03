/*
 * RadarListener.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: Jad
 */

#include "RadarListener.h"
#include "Hit.h"
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
using namespace std;

RadarListener::RadarListener(Airspace* airspace, pthread_attr_t* threadAttr) {
	this->airspace = airspace;
	this->threadAttr = threadAttr;
}

RadarListener::~RadarListener() {
	this->kill();
	pthread_attr_destroy(threadAttr);
}

const pthread_t* RadarListener::run() {
	if (pthread_create(&radarListener, threadAttr, (RADAR_FUNC_PTR) &RadarListener::populateAirspace, this) == 0){
			return &radarListener;
		} else
			return nullptr;

}

void *RadarListener::populateAirspace(void *) {

	vector<Hit> hitList;
	int timer = 0;
	string id, vx, vy, vz, x, y, z, eTime;
	int aircraft_id;
	int i = 0;
	Hit hit;
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
					hit.setData(aircraft_id, stoi(vx), stoi(vy), stoi(vz), stoi(x), stoi(y), stoi(z), stoi(eTime));
					hitList.push_back(hit);
					//airspace->addAircraft(hit);
					//cout << '(' << id << ',' << vx << ',' << vy << ',' << vz << ',' << x << ',' << y << ',' << z << ',' << eTime << ')' << endl;
				}
		}
	} else {
		cout << "File could not open." << endl;
	}

	while(!killFlag) {

		for(Hit x : hitList) {

			cout << "\n\nENTRY_TIME: " << x.getEntryTime() << "\tTIMER: " << timer << endl;
			int erase = 0;

			if(x.getEntryTime() <= timer) {

				if(x.getId() > 0) {
					airspace->addAircraft(x);
				}
				erase++;
			}

			while(erase > 0) {
				hitList.erase(hitList.begin());
				erase--;
			}
		}

		for(Hit y : hitList) {
			cout << '(' << y.getId() << ',' << y.getSpeedx() << ',' << y.getSpeedy() << ',' << y.getSpeedz() << ',' << y.getLocationx() << ',' << y.getLocationy() << ',' <<y.getLocationz() << ',' << y.getEntryTime() << ')' << endl;
		}
		//sleep thread for 5 seconds
		//replace with a real timer
		timer += 5;
	}
	return nullptr;
}

void RadarListener::kill(){
	killFlag = true;
	pthread_sleepon_lock();
	pthread_sleepon_unlock();
	pthread_join(radarListener, NULL);
}
