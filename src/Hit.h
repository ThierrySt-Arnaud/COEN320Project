/*
 * Hit.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */

#ifndef HIT_H_
#define HIT_H_
#include "Coordinates.h"

class Hit : public Coordinates {
public:
	//Default constructor
	Hit();
	//Destructor
	~Hit();
	//Getters
	int getId() {
		return aircraft_id;
	}
	//Setters
	void setId(int id) {
		aircraft_id = id;
	}

private:
	//Hit variables
	int aircraft_id;
};

#endif /* HIT_H_ */
