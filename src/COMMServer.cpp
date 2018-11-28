/*
 * COMMServer.cpp
 *
 *  Created on: Nov 24, 2018
 *      Author: thierry
 */


#include <confname.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/stat.h>
#include <string>
#include <string.h>

#include "OperatorCommand.h"
#include "CommMessage.h"

#define COMM_OUT "./CommOut.txt"

pthread_mutex_t outgoingMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t commandMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutexattr_t incomingMutexAttr;
pthread_mutexattr_t outgoingMutexAttr;
pthread_mutexattr_t commandMutexAttr;


pthread_t commsender;
pthread_t kblistener;
pthread_t cmdinterpreter;

std::queue<OperatorCommand> commandQueue;
std::queue<CommMessage> outgoingQueue;

void* commSender(void *);

void* kbListener(void *);

void* cmdInterpreter(void *);

int main(){

	pthread_mutexattr_init( &outgoingMutexAttr );
	pthread_mutexattr_init( &commandMutexAttr );

	pthread_mutexattr_setwakeup_np( &outgoingMutexAttr, PTHREAD_WAKEUP_ENABLE );

	if (pthread_mutex_init(&outgoingMutex, &outgoingMutexAttr) == -1 ||
		pthread_mutex_init(&commandMutex, &commandMutexAttr) == -1)
	{
			std::cerr << "Err " + std::to_string(errno) + ", unable to initialize mutex. " + strerror(errno);
			return -1;
	}

	outgoingQueue = queue();
	commandQueue = queue();

	pthread_create(&commsender,NULL,&commSender,NULL);
	pthread_create(&kblistener,NULL,&kbListener,NULL);
	pthread_create(&cmdinterpreter,NULL,&cmdInterpreter,NULL);



}

void* commSender(void * attr){
	std::ofstream commOut;
	commOut.open(COMM_OUT, ios_base::trunc | out);

	commOut.close();
	return NULL;
}

void* kbListener(void *attr){

	return NULL;
}

void* cmdInterpreter(void *attr){

	return NULL;
}
