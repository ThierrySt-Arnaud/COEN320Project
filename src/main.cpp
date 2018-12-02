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
#include "KeyboardListener.h"
#include "CommServer.h"
#include "RadarListener.h"
#include "Airspace.h"
#include <sstream>
#include <iostream>

void ncursesSetup(int[]);
//void populateAirspace(string);

int main(){

	/*populateAirspace("/home/qnxuser/ATC/RadarIn.txt");

		for(Hit x : airspace.getAircrafts()) {
			cout << '(' << x.getId() << ',' << x.getSpeedx() << ',' << x.getSpeedy() << ',' << x.getSpeedz() << ',' << x.getLocationx() << ',' << x.getLocationy() << ',' <<x.getLocationz() << ',' << x.getEntryTime() << ')' << endl;
		}*/

	int screenSize[2];
	ncursesSetup(screenSize);

	CommServer commServer;
	Airspace airspace;
	KeyboardListener keyboardListener(&airspace, &commServer, screenSize);
	RadarListener radarListener(&airspace);

	radarListener.run();
	commServer.run();
	pthread_join(keyboardListener.run(), NULL);
	endwin();
	fflush(stdout);
	commServer.kill();
	radarListener.kill();
	return 0;
}

void ncursesSetup(int screenSize[]){
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	curs_set(0);
	getmaxyx(stdscr, screenSize[0], screenSize[1]);
}
/*
void populateAirspace(string inputFile) {

	string id, vx, vy, vz, x, y, z, eTime;
	int aircraft_id;
	int i = 0;
	Hit hit;
	string line;
	ifstream inFile;
	inFile.open(inputFile, ios::in);

	if (inFile.is_open()) {
			while(getline(inFile, line)) {
				if(line.find("int") != std::string::npos or line.find("}") != std::string::npos or line.empty()) {
					//do nothing
				} else {
					stringstream ss(line);
					getline(ss,id,',');
					getline(ss,vx,',');
					getline(ss,vy,',');
					getline(ss,vz,',');
					getline(ss,x,',');
					getline(ss,y,',');
					getline(ss,z,',');
					getline(ss, eTime,',');
					aircraft_id = stoi(id);
					if(aircraft_id == -1) {
						i--;
						aircraft_id = i;
					}
					hit.setData(aircraft_id, stoi(vx), stoi(vy), stoi(vz), stoi(x), stoi(y), stoi(z), stoi(eTime));
					airspace.addAircraft(hit);
					//cout << '(' << id << ',' << vx << ',' << vy << ',' << vz << ',' << x << ',' << y << ',' << z << ',' << eTime << ')' << endl;
				}
		}
	} else {
		cout << "File could not open." << endl;
	}
}*/
