#ifndef HUMAN_H
#define HUMAN_H
#include <unordered_map>
#include "MainDarkLogic/player.h"
#include "MainDarkLogic/EnumFun.h"

class Human : public Player
{
public:
	Human();

	std::shared_ptr<const Action> play() override;

	~Human() = default;

private:
	static const std::unordered_map<std::string, Fun> s_funHash;
};


#endif // !HUMAN_H

