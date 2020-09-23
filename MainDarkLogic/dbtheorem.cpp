#include "dbtheorem.h"
#include <random>
#include <ctime>
#include <stdexcept>

DbTheorem::Theorem::Theorem(const std::string& name_, const std::string& content_, unsigned short int elo_):
	m_name(name_), m_content(content_), m_elo(elo_)
{
}

std::string DbTheorem::Theorem::name() const
{
	return m_name;
}

std::string DbTheorem::Theorem::content() const
{
	return m_content;
}

unsigned short int DbTheorem::Theorem::elo() const
{
	return m_elo;
}

DbTheorem::DbTheorem()
{
	std::srand(std::time(nullptr));
	add("Non-contradiction", "!(a&&!a)", 500);
	add("Semi-Identity", "a=>a", 700);
	add("Semi-DoubleNot", "a=>!!a", 750);
	add("Identity", "a<=>a", 1200);
	add("DoubleNot", "a<=>!!a", 1450);
	add("Identity and True", "(a<=>a)&&True", 1750);
	add("Double-Not and True", "(a<=>!!a)&&True", 1750);
	add("Semi-Identity and Semi-Double-Not", "(a=>a)&&(a=>!!a)", 1800);
	add("Contraposition", "(a=>b)=>(!b=>!a)", 1850);
	add("Identity-Double-Not", "(a<=>a)&&(a<=>!!a)", 2350);
	add("ExcludedMiddle", "a||!a", 2550);
	add("Peirce law", "((a=>b)=>a)=>a", 2550);
	add("Modus ponens", "((a=>b)&&a)=>b", 2550);
	add("De Morgan law", "!(a&&b)<=>(!a||!b)", 2650);
	add("De Morgan law (2)", "!(a||b)<=>(!a&&!b)", 2750);
	add("Modus tollens", "((a=>b)&&!b)=>!a", 2800);
	add("Modus barbara", "((a=>b)&&(b=>c))=>(a=>c)", 2850);
	add("&& Distributivity", "(a&&(b||c))<=>((a&&b)||(a&&c))", 2850);
	add("|| Distributivity", "(a||(b&&c))<=>((a||b)&&(a||c))", 2850);
}

void DbTheorem::add(const std::string& name_, const std::string& content_, unsigned short int elo_)
{
	if (!m_db.contains(content_))
	{
		m_db.insert({ content_, DbTheorem::Theorem(name_, content_, elo_) });
	}
}

const DbTheorem::Theorem DbTheorem::getRandomTheorem(unsigned short int elo) const
{
	std::vector<std::string> allKeys;
	std::vector<std::string> remainKeys;
	for (auto& it : m_db)
	{
		allKeys.push_back(it.first);
		auto thm = it.second;
		if ((elo - MAX_ELO_RANGE <= thm.elo()) && (thm.elo() <= elo + MAX_ELO_RANGE))
		{
			remainKeys.push_back(it.first);
		}		
	}
	if (remainKeys.size() > 0)
	{
		return m_db.at(remainKeys[rand() % remainKeys.size()]);
	}
	else
	{
		return m_db.at(allKeys[rand() % allKeys.size()]);
	}
}