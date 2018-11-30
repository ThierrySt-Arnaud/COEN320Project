/*
 * KeyboardListener.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#include "KeyboardListener.h"

KeyboardListener::KeyboardListener(Airspace* airspace, CommServer* commServer, pthread_attr_t* threadAttr){
	this->airspace = airspace;
	this->commserver = commServer;
	this->threadAttr = threadAttr;
}

KeyboardListener::~KeyboardListener() {
	pthread_attr_destroy(threadAttr);
	this->kill();
}

const pthread_t* KeyboardListener::run(){
	if (pthread_create(&cmdInterpreter, threadAttr, (KBLISTENER_FUNC_PTR) &KeyboardListener::commandInterpreter, this) == 0){
		return &kbListener;
	} else
		return nullptr;
}

void* KeyboardListener::keyboardListener(void*){
	while(!killFlag){

	}
	return nullptr;
}

void* KeyboardListener::commandInterpreter(void*){
	return nullptr;
}

void KeyboardListener::kill(){
	killFlag = true;
}
