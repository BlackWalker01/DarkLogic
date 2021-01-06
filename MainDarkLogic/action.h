#ifndef ACTION_H
#define ACTION_H
#include "EnumFun.h"
#include <memory>
#include "utils.h"

class Action
{
public:
	Action(const Fun fun_);
	Action(const Fun fun_, const Id& id_);

	Fun fun() const;
	Id id() const;

	~Action() = default;
private:
	const Fun m_fun;
	const std::unique_ptr<Id> m_id;
};

#endif 