/*
 * Airspace.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef AIRSPACE_H_
#define AIRSPACE_H_

#include <vector>
#include "Hit.h"

class Airspace {
public:
	Airspace();
	virtual ~Airspace();
	std::vector<Hit> addAircraft(std::vector<Hit>, Hit);
	std::vector<Hit> remAircraft(std::vector<Hit> vector, int id);
	std::vector<Hit> editAircraft(std::vector<Hit> vector, int id, int x, int y, int z);
	void display(std::vector<Hit> vector);
private:
	std::vector<Hit> airspace;

};

#endif /* AIRSPACE_H_ */
