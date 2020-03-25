#include "player.h"

Player::Player(const std::string& name_):
	m_name(name_)
{

}

std::string Player::name() const
{
	return m_name;
}
