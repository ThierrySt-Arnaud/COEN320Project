/*
 * RadarListener.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef RADARLISTENER_H_
#define RADARLISTENER_H_

class RadarListener {
public:

	pthread_mutex_t airspaceMutex;
	pthread_t radarListener;
	Airspace* airspace;
	RadarListener(Airspace*, pthread_attr_t* = nullptr, pthread_mutexattr_t* = nullptr);
	virtual ~RadarListener();
	pthread_t* run();
	bool kill();
};

#endif /* RADARLISTENER_H_ */
