/*
 * RadarListener.h
 *
 *  Created on: Dec 1, 2018
 *      Author: Jad
 */

#ifndef SRC_RADARLISTENER_H_
#define SRC_RADARLISTENER_H_

#include <fstream>
#include <pthread.h>
#include <atomic>
#include <queue>
#include <string>
#include "Airspace.h"
using namespace std;

typedef void * (*RADAR_FUNC_PTR)(void *);

#define INPUT_FILE "/home/qnxuser/ATC/RadarIn.txt"

class RadarListener {
public:
	RadarListener(Airspace*, pthread_attr_t* = nullptr);
	RadarListener(const RadarListener&) = delete;
	virtual ~RadarListener();
	pthread_t run();
	void kill();
private:
	pthread_attr_t* threadAttr;
	pthread_mutex_t radarMutex;
	pthread_mutexattr_t radarMutexAttr;
	pthread_t radarListener = false;
	Airspace* airspace;
	atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void *populateAirspace(void *);
};

#endif /* SRC_RADARLISTENER_H_ */
