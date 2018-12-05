/*
 * Airspace.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include "Airspace.h"
using namespace std;

Airspace::Airspace() {
	// TODO Auto-generated constructor stub

}

Airspace::~Airspace() {
}


void Airspace::addAircraft(Hit hit) {
	pthread_mutex_lock(&airspace_mutex);
	airspace.insert(std::pair<int, Hit>(hit.getId(), hit));
	pthread_mutex_unlock(&airspace_mutex);
}

vector<Hit> Airspace::getAircrafts(){
	vector<Hit> outputAirspace;
	pthread_mutex_lock(&airspace_mutex);
	for (const auto &s: airspace){
		outputAirspace.push_back(s.second);
	}
	pthread_mutex_unlock(&airspace_mutex);
	return outputAirspace;
}

void Airspace::remAircraft(int id) {
	pthread_mutex_lock(&airspace_mutex);
	airspace.erase(id);
	pthread_mutex_unlock(&airspace_mutex);
}

void Airspace::chgAircraftAlt(int id, int newAlt) {
	pthread_mutex_lock(&airspace_mutex);
	try{
		airspace.at(id).setAlt(newAlt);
	} catch(std::out_of_range& e){
		// The ID wasn't found, but we don't care
	}
	pthread_mutex_unlock(&airspace_mutex);
}

void Airspace::chgAircraftSpd(int id, std::array<int, 3> newSpd) {
	pthread_mutex_lock(&airspace_mutex);
	try{
		airspace.at(id).setSpd(newSpd);
	} catch(std::out_of_range& e){
		// The ID wasn't found, but we don't care
	}
	pthread_mutex_unlock(&airspace_mutex);
}

void Airspace::setColliding(int id, int collider){
	pthread_mutex_lock(&airspace_mutex);
	try{
		airspace.at(id).setCollision(collider);
		airspace.at(collider).setCollision(id);
	} catch(std::out_of_range& e){
		// The ID wasn't found, but we don't care
	}
	pthread_mutex_unlock(&airspace_mutex);
}

void Airspace::updateAircrafts() {
	pthread_mutex_lock(&airspace_mutex);
	for (auto &s : airspace){
		s.second.updateLocation();
	}
	pthread_mutex_unlock(&airspace_mutex);
}
