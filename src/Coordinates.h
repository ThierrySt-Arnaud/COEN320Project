/*
 * Coordinates.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Jad
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_

class Coordinates {
public:
	//Default constructor
	Coordinates();
	//Overload constructor
	Coordinates(int,int,int);
	//Getters
	int getXCoordinates() {
		return x;
	}
	int getYCoordinates() {
		return y;
	}
	int getZCoordinates() {
		return z;
	}
	//Setters
	void setCoordinates(int a, int b, int c) {
		x = a;
		y = b;
		z = c;
	}
private:
	int x;
	int y;
	int z;
};

#endif /* COORDINATES_H_ */
