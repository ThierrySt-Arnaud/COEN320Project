/*
 * UpdateAirspace.h
 *
 *  Created on: Dec 2, 2018
 *      Author: Jad
 */

#ifndef SRC_UPDATEAIRSPACE_H_
#define SRC_UPDATEAIRSPACE_H_


#include <pthread.h>
#include <atomic>
#include <string>
#include "Airspace.h"
#include "CommServer.h"
using namespace std;

typedef void * (*UPDATE_FUNC_PTR)(void *);
const int MAX_X = 100000;
const int MAX_Y = 100000;
const int MAX_Z = 25000;
const int MIN_DIST = 1000;
const int MIN_DIST_SQR = MIN_DIST*MIN_DIST;
const int COLL_EST_TIME = 5;

//100 000 x 100 000 x 25 000

class UpdateAirspace {
public:
	UpdateAirspace(Airspace*, CommServer*, pthread_attr_t* = nullptr);
	UpdateAirspace(const UpdateAirspace&) = delete;
	virtual ~UpdateAirspace();
	pthread_t run();
	void kill();
private:
	pthread_attr_t* threadAttr;
	pthread_mutex_t updateMutex;
	pthread_mutexattr_t updateMutexAttr;
	pthread_t updateAirspace = false;
	Airspace* airspace;
	CommServer* commServer;
	atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void *update(void *);
	void outOfBounds();
	void collisionCheck();
};

#endif /* SRC_UPDATEAIRSPACE_H_ */
