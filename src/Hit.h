/*
 * Hit.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */

#ifndef HIT_H_
#define HIT_H_
#include "Coordinates.h"
using namespace std;

class Hit : public Coordinates {
public:
	//Default constructor
	Hit();
	//Destructor
	~Hit();
	//Getters
	void setData(int id, int vx, int vy, int vz, int x, int y, int z, int eTime) {
		aircraft_id = id;
		speedx = vx;
		speedy = vy;
		speedz = vz;
		locationx = x;
		locationy = y;
		locationz = z;
		entryTime = eTime;
		}
	int getId() {
		return aircraft_id;
	}
	int getSpeedx() {
		return speedx;
	}
	int getSpeedy() {
		return speedy;
	}
	int getSpeedz() {
		return speedz;
	}
	int getLocationx() {
		return locationx;
	}
	int getLocationy() {
		return locationy;
	}
	int getLocationz() {
		return locationz;
	}
	int getEntryTime() {
		return entryTime;
	}

private:
	//Hit variables
	int aircraft_id;
	int speedx;
	int speedy;
	int speedz;
	int locationx;
	int locationy;
	int locationz;
	int entryTime;
};

#endif /* HIT_H_ */
