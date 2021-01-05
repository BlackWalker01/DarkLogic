#include "action.h"

Action::Action(const Fun fun_):
	m_fun(fun_), m_id(nullptr)
{

}

Action::Action(const Fun fun_, const Id& id_):
	m_fun(fun_), m_id(std::make_unique<Id>(id_))
{
}

Fun Action::fun() const
{
	return m_fun;
}

Id Action::id() const
{
	if (m_id)
	{
		return *m_id;
	}
	return std::numeric_limits<Id>::max();
}
