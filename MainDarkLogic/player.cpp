#include "player.h"
#include <iostream>

Player::Player(const std::string& name_, const size_t& nbGames, unsigned short int elo_):
	m_name(name_), m_nbGames(nbGames), m_elo(elo_)
{

}

std::string Player::name() const
{
	return m_name;
}

size_t Player::nbGames() const
{
	return m_nbGames;
}

unsigned short int Player::elo() const
{
	return m_elo;
}

void Player::setElo(unsigned short int elo_)
{
	auto exElo = m_elo;
	m_elo = elo_;
	if (exElo <= elo_)
	{
		std::cout << m_name << "'s elo is " << m_elo << " (+" << elo_ - exElo << ")" << std::endl;
	}
	else
	{
		std::cout << m_name << "'s elo is " << m_elo << " (-" << exElo - elo_ << ")" << std::endl;
	}
	m_elo++;
}

void Player::meditate()
{
	m_nbGames++;
}
