/*
 * UpdateAirspace.h
 *
 *  Created on: Dec 2, 2018
 *      Author: Jad
 */

#ifndef SRC_UPDATEAIRSPACE_H_
#define SRC_UPDATEAIRSPACE_H_

#include <fstream>
#include <pthread.h>
#include <atomic>
#include <queue>
#include <string>
#include "Airspace.h"
using namespace std;

typedef void * (*UPDATE_FUNC_PTR)(void *);

class UpdateAirspace {
public:
	UpdateAirspace(Airspace*, pthread_attr_t* = nullptr);
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
	atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void *update(void *);
};

#endif /* SRC_UPDATEAIRSPACE_H_ */
