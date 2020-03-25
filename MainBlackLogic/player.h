#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <memory>
//#include "MainBlackLogic/action.h"

class Action;

class Player
{
public:
	Player(const std::string& name_);

	std::string name() const;
	virtual std::shared_ptr<const Action> play() = 0;

	virtual ~Player() = default;	

protected:
	const std::string m_name;
};

#endif
