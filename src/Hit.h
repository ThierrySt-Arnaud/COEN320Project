/*
 * Hit.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */

#ifndef HIT_H_
#define HIT_H_
#include <array>
using namespace std;

class Hit{
public:
	//Default constructor
	Hit(int, int, int, int, int, int, int, int);
	//Destructor
	~Hit();
	//Getters
	void setLocation(std::array<int, 3>);
	void setAlt(int);
	void setSpd(std::array<int, 3>);
	void setCollision(int);
	void updateLocation();
	int getId();
	std::array<int, 3> getSpeed();
	std::array<int, 3> getLocation();
	bool colliding();
	int getCollider();
	int getEntryTime();

private:
	//Hit variables
	int aircraft_id;
	std::array<int, 3> speed;
	std::array<int, 3> location;
	bool collidingFlag;
	int collider;
	int entryTime;
};

#endif /* HIT_H_ */
