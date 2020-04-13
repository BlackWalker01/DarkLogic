#include "action.h"

Action::Action(const Fun fun_):
	m_fun(fun_), m_id(nullptr)
{

}

Action::Action(const Fun fun_, const size_t& id_):
	m_fun(fun_), m_id(std::make_unique<size_t>(id_))
{
}

Fun Action::fun() const
{
	return m_fun;
}

size_t Action::id() const
{
	if (m_id)
	{
		return *m_id;
	}
	return std::numeric_limits<size_t>::max();
}
