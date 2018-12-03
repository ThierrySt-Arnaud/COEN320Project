/*
 * AirspaceDisplay.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: thierry
 */

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "Airspace.h"
#include "AirspaceDisplay.h"

AirspaceDisplay::AirspaceDisplay(Airspace* airspace, int screenSize[], pthread_attr_t* threadAttr) {
	this->airspace = airspace;
	this->threadAttr = threadAttr;
	memcpy(this->screenSize, screenSize, sizeof(this->screenSize));
	pthread_mutexattr_init(&this->dpTimerMutexAttr);
	pthread_mutexattr_setprotocol(&this->dpTimerMutexAttr,PTHREAD_PRIO_NONE);
	pthread_mutex_init(&this->dpTimerMutex, &this->dpTimerMutexAttr);
	display_win = newwin(this->screenSize[0]-4, this->screenSize[1], 0, 0 );
}

AirspaceDisplay::~AirspaceDisplay() {
	this->kill();
	pthread_attr_destroy(threadAttr);
}

pthread_t AirspaceDisplay::run(){
	pthread_mutex_lock(&dpTimerMutex);
	if (pthread_create(&dprefresher, threadAttr, (DISPLAYREFRESHER_FUNC_PTR) &AirspaceDisplay::displayRefresher, this) == 0){
		return dprefresher;
	} else {
		pthread_mutex_unlock(&dpTimerMutex);
		return -1;
	}
}

void AirspaceDisplay::kill(){
	killFlag = true;
	pthread_mutex_unlock(&dpTimerMutex);
	pthread_join(dprefresher, NULL);
}

void* AirspaceDisplay::displayRefresher(void *){
	struct timespec timeout;
	while (!killFlag){
		vector<Hit>aircrafts = airspace->getAircrafts();
		wclear(display_win);
		int curY = 0, curX = 0;
		for(Hit h : aircrafts){
			int ID = h.getId();
			char* idStatus;
			if (ID < 0){
				idStatus = "UFO";
				ID *= -1;
			} else {
				idStatus = "Flight";
			}
			int x = h.getLocationx(), y = h.getLocationy(), z = h.getLocationz(),
					vx = h.getSpeedx(), vy = h.getSpeedy(), vz = h.getSpeedz();
			mvwprintw(display_win, curY++, curX ,"%s #%i Pos: %i, %i, %i Spd: %i, %i, %i",
					  idStatus, ID, x, y, z, vx, vy, vz);
		}
		wrefresh(display_win);
		clock_gettime(CLOCK_MONOTONIC, &timeout);
		timeout.tv_sec += 1;
		pthread_mutex_timedlock_monotonic(&dpTimerMutex, &timeout);
	}
	wclear(display_win);
	wrefresh(display_win);
	wclear(stdscr);
	wrefresh(stdscr);
	delwin(display_win);
	return nullptr;
}
