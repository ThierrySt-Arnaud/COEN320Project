/*
 * EventMessage.h
 *
 *  Created on: Nov 25, 2018
 *      Author: thierry
 */

#ifndef OPERATORCOMMAND_H_
#define OPERATORCOMMAND_H_

#include <array>

enum OpCmdType {  ALT_CHG, SPD_CHG, DIR_CHG, TOG_HOLD, PING };

class OperatorCommand{
public:
	OperatorCommand(OpCmdType, std::array<int, 4>);
	OpCmdType getType();
	void setType(OpCmdType);
	std::array<int, 4> getContent;
	void setContent(std::array<int, 4>);
private:
	OpCmdType type;
	std::array<int, 4> content;
};

#endif /* OPERATORCOMMAND_H_ */
