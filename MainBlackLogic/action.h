#ifndef ACTION_H
#define ACTION_H
#include "EnumFun.h"
#include <memory>

class Action
{
public:
	Action(const Fun fun_);
	Action(const Fun fun_, const size_t& id_);

	Fun fun() const;
	size_t id() const;

	~Action() = default;
private:
	const Fun m_fun;
	const std::unique_ptr<size_t> m_id;
};

#endif 