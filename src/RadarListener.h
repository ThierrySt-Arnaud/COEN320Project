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
using namespace std;

typedef void * (*RADAR_FUNC_PTR)(void *);

class RadarListener {
public:
	RadarListener(pthread_attr_t* = nullptr);
	RadarListener(const RadarListener&) = delete;
	virtual ~RadarListener();
	const pthread_t* run();
	void populateAirspace(string);
	void kill();
private:
	pthread_attr_t* threadAttr;
	pthread_mutex_t outgoingMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t radarListener = false;
	atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void *updateAirspace(void *);
};

#endif /* SRC_RADARLISTENER_H_ */
