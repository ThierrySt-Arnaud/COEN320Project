/*
 * COMMServer.cpp
 *
 *  Created on: Nov 24, 2018
 *      Author: thierry
 */





/*
 *  sendrec.c
 *
 *  This process demonstrates Neutrino messaging by creating a
 *  client thread and a server thread all in the same process.
 *
 *  Why would you do this?  Consider the case where you have a
 *  process that has to spend most of its time blocked on MsgReceive()
 *  ready to handle time critical messages.   However, it may also
 *  want to get data from a serial port.  The standard way to get
 *  data from a serial port is to do a read().  So how can it block
 *  on MsgReceive() and do read()s as well?  It could create a
 *  worker thread who will block on read() for it.  When serial
 *  data arrives, the worker thread would send it to the receiving
 *  thread.
 *
 *  To test it, run it as follows:
 *    sendrec
 *    sendrec [name_of_device_to_read_from]
 *
 *  For example:
 *    sendrec /dev/con1
 *
 */
#include <confname.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>



void* receiver(void *);

int main(){
	const char* queueName = "/commchannel";
	mqd_t mq_open(queueName,
					O_CREAT | O_RDWR);
	printf("Queue \"%s\" has been opened.\n", queueName);
	mq_unlink(queueName);
	printf("Queue \"%s\" has been closed.\n", queueName);
}

void* receiver(void * attr){
	return NULL;
}
