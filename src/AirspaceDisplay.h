/*
 * AirspaceDisplay.h
 *
 *  Created on: Dec 2, 2018
 *      Author: thierry
 */

#ifndef SRC_AIRSPACEDISPLAY_H_
#define SRC_AIRSPACEDISPLAY_H_

#include <pthread.h>
#include <ncurses.h>
#include <atomic>

typedef void * (*DISPLAYREFRESHER_FUNC_PTR)(void *);

class AirspaceDisplay {
public:
	AirspaceDisplay(Airspace*, int[], pthread_attr_t* = nullptr);
	virtual ~AirspaceDisplay();
	pthread_t run();
	void kill();
private:
	void* displayRefresher(void*);
	WINDOW* display_win;
	pthread_t dprefresher = false;
	pthread_mutex_t dpTimerMutex;
	pthread_mutexattr_t dpTimerMutexAttr;
	pthread_attr_t* threadAttr;
	Airspace* airspace;
	int screenSize[2];
	std::atomic_bool killFlag = ATOMIC_VAR_INIT(false);
};

#endif /* SRC_AIRSPACEDISPLAY_H_ */
