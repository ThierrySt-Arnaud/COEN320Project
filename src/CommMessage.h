/*
 * CommMessage.h
 *
 *  Created on: Nov 25, 2018
 *      Author: thierry
 */

#ifndef COMMMESSAGE_H_
#define COMMMESSAGE_H_

#include <string>

enum CommMsgType { REPORT, SPDREQUEST, ALTREQUEST, HANDOFF, LOG };

class CommMessage{
public:
	CommMsgType type;
	int remoteID;
	std::string content;
	CommMessage(CommMsgType, std::string, int);
	CommMsgType getType();
	void setType(CommMsgType);
	int getRemoteID();
	void setRemoteID(int);
	std::string getContent();
	void setContent(std::string);
};

#endif /* COMMMESSAGE_H_ */
