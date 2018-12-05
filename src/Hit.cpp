/*
 * Hit.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */
#include <iostream>
#include "Hit.h"
//#include <vector>
//#include <string>
using namespace std;

Hit::Hit(int id, int vx, int vy, int vz, int x, int y, int z, int eTime) {
	aircraft_id = id;
	speed[0] = vx;
	speed[1] = vy;
	speed[2] = vz;
	location[0] = x;
	location[1] = y;
	location[2] = z;
	entryTime = eTime;
	collidingFlag = false;
	collider = 0;
}

Hit::~Hit() {

}

void Hit::setLocation(std::array<int, 3> newLoc){
	location = newLoc;
}

void Hit::setAlt( int newAlt){
	location[2] = newAlt;
	collidingFlag = false;
}

void Hit::setSpd(std::array<int, 3> newSpd){
	speed = newSpd;
	collidingFlag = false;
}

void Hit::setCollision(int collider){
	collidingFlag = true;
	this->collider = collider;
}

void Hit::updateLocation(){
	location[0] += speed[0];
	location[1] += speed[1];
	location[2] += speed[2];
}

int Hit::getId() {
	return aircraft_id;
}

std::array<int, 3> Hit::getSpeed() {
	return speed;
}

std::array<int, 3> Hit::getLocation() {
	return location;
}

bool Hit::colliding(){
	return collidingFlag;
}

int Hit::getCollider(){
	return collider;
}

int Hit::getEntryTime() {
	return entryTime;
}
