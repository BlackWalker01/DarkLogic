#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <memory>
//#include "MainBlackLogic/action.h"

class Action;

class Player
{
public:
	Player(const std::string& name_, const size_t& nbGames = 0, unsigned short int elo_ = 1000);

	std::string name() const;
	size_t nbGames() const;
	unsigned short int elo() const;
	void setElo(unsigned short int elo_);
	virtual std::shared_ptr<const Action> play() = 0;
	virtual void meditate();

	virtual ~Player() = default;	

protected:
	const std::string m_name;
	size_t m_nbGames;
	unsigned short int m_elo;
};

#endif
