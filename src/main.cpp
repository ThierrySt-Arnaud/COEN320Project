/*
 * main.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */

#include <confname.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include "CommServer.h"
#include <sstream>


int main(){
	CommServer commServer;

	commServer.run();

	std::stringstream test2content;
	test2content << 1000 << ' ' << 1000 << ' ' << 1000 << ' ' << 1000 ;

	CommMessage test1 = CommMessage(REPORT, " ", 0);
	CommMessage test2 = CommMessage(REQUEST, test2content.str(), 10);
	CommMessage test3 = CommMessage(HANDOFF, " ", 10);

	commServer.send(test1);
	commServer.send(test2);
	commServer.send(test3);

	commServer.kill();
}
