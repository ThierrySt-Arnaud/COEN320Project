/*
 * KeyboardListener.h
 *
 *  Created on: Nov 27, 2018
 *      Author: thierry
 */

#ifndef KEYBOARDLISTENER_H_
#define KEYBOARDLISTENER_H_

#include <queue>
#include <pthread.h>
#include <fstream>
#include <atomic>
#include "OperatorCommand.h"
#include "Airspace.h"

typedef void * (*KBLISTENER_FUNC_PTR)(void *);
typedef void * (*CMDINTERPRETER_FUNC_PTR)(void *);

class KeyboardListener {
public:

	KeyboardListener(Airspace*, CommServer*, pthread_attr_t* = nullptr);
	virtual ~KeyboardListener();
	const pthread_t* run();
	void kill();
private:
	pthread_mutex_t commandMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t kbListener = false, cmdInterpreter = false;
	pthread_attr_t* threadAttr;
	Airspace* airspace;
	CommServer* commserver;
	std::queue<OperatorCommand> commandQueue;
	std::atomic_bool killFlag = ATOMIC_VAR_INIT(false);
	void* keyboardListener(void*);
	void* commandInterpreter(void*);
};

#endif /* KEYBOARDLISTENER_H_ */
